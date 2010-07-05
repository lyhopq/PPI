#include <math.h>

#include "ppisec.h"

#include <QDebug>


extern sysValue *sysval;

/*!
*    \brief 构造函数
*/
PPISec::PPISec( PPIPainter *p )
{
    painter = p;

    dispInfo = sysval->getDispInfo();
    headAzi = 0;


    //******************************
    // 模拟目标
    memset(sec_track,0,sizeof(sec_track));
     //批号
    sec_track[0] = 0x01;
    sec_track[1] = 0x01;
    //目标方位
    sec_track[2] = 0x0f;
    sec_track[3] = 0x00;
    //距离
    sec_track[4] = 0x0f;
    sec_track[5] = 0x00;
    //目标航向
    sec_track[6] = 0x0f;
    sec_track[7] = 0x00;

    secTimer=new QTimer(this);
    connect( secTimer,SIGNAL( timeout()),this,SLOT( secTimeOut()));
    secTimer->setSingleShot(false);
    secTimer->start(500); // 100ms
    //******************************
}

//! 析构函数
PPISec::~PPISec()
{
    //if(pSectorRegion!=NULL) delete pSectorRegion;
    //if(pWarnSectorRegion!=NULL) delete pWarnSectorRegion;

    //delete sysValue;

}


/*!
*    \brief 绘制二次显示画面
*
*    绘制坐标、刻度、距离圈和二次信息
*/
void PPISec::draw()
{
    //qDebug("dispInfo->centerX=%d,dispInfo->centerY=%d,dispInfo->radiusPPI=%d\n",dispInfo->centerX,dispInfo->centerY,dispInfo->radiusPPI);

    painter->setColor(FB_GREEN);
    painter->drawDistantCircle(dispInfo->centerX,dispInfo->centerY);
    painter->drawCircle(dispInfo->centerX,dispInfo->centerY,dispInfo->radiusPPI);

    painter->drawCrossLine(dispInfo->centerX,dispInfo->centerY,dispInfo->radiusPPI);
    painter->drawScale(dispInfo->centerX,dispInfo->centerY);

    //画舰首线
    painter->setColor(FB_BLUE);
    painter->drawLine(dispInfo->centerX,dispInfo->centerY,headAzi,dispInfo->radiusPPI);

    /*
    //画扇区
    pSectorRegion->drawAllSectorRegion();
    pWarnSectorRegion->drawAllSectorRegion();
    */
    //画二次信息
    if(dispInfo->bSecondInfo)
        drawAllTrack();
    /*
    //显示告警
    if(pWarnSectorRegion->getWarnFlag())
        emit(showWarnFlag(1));
    */
}

/*!
*    \brief 模拟二次数据
*
*    模拟产生二次数据，送函数addTrack(unsigned char* track)处理。
*/
void PPISec::secTimeOut()
{
    // 距离
    unsigned short *d = (unsigned short *)&sec_track[4];
    static bool flag = true;
    if(*d == 0x000f)
    {
        flag = true;
    }
    else if(*d == 0x03ff)
    {
        flag = false;
    }
    *d = flag ? *d+16 : *d-16;

    // 方位
    unsigned short *f = (unsigned short *)&sec_track[2];
    if(*f == 0x0fff)
    {
        *f = 0x000f;
        sec_track[0]++;      // 批号
        sec_track[1] = 0x02; // 属性
    }
    *f += 16;

    // 航向
    unsigned short *h = (unsigned short *)&sec_track[6];
    if(*h == 0x0fff)
        *h = 0x0f;
    *h += 16;

    addTrack(sec_track);
}

/*!
*    \brief 增加航迹
*
*    \param track    目标信息
*
*    根据目标信息进行航迹处理。
*/
void PPISec::addTrack(unsigned char *track)
{
   if( dispInfo->bFreeze == true) // 画面冻结
        return;

    unsigned int batchNum = track[0] + (track[1]>>4)*256;  // 批号
    unsigned char trackattr  = (track[1] & 0x03);     // 属性

    //方位
    int angle = track[3] & 0x0f;
    angle = angle << 8;
    angle = angle + track[2];
    double fw = (angle/4096.0)*3.14159*2.0;

    //航向
    angle = track[7] & 0x0f;
    angle = angle << 8;
    angle = angle + track[6];

    //距离
    int jlint = track[4] + (track[5] & 0x03) * 256;
    if( jlint >= (dispInfo->rangePPI<<1))//超出量程   x2 采集的距离是实际的2倍
        return;
    double jl = jlint * (double)dispInfo->radiusPPI / ((double)dispInfo->rangePPI * 2.0);
    //由方位和距离算出坐标
    double x =  dispInfo->centerX + jl * (sin(fw));
    double y =  dispInfo->centerY - jl * (cos(fw));

    //查找是否批号已存在
    bool bexit = false;
    for( ite = vectck.begin(); ite!=vectck.end(); ite++ )
    {
        if( ite->index() == batchNum )
        {
            bexit = true;
            break;
        }
    }

    if(dispInfo->bSecondInfo)//是否显示二次信息
    {
        if( bexit )//批号已经存在,增加新的点迹
        {
            /*
            if(batchNum >= 300)//手动录取的目标
            {
                double dx = x - *(ite->xend()-1);
                double dy = y - *(ite->yend()-1);
                double degree = atan2((double)dx , -(double)dy);
                if(degree < 0)
                    degree = PI + PI + degree;
                angle = (int)((degree*2048.0) / PI);
            }
            */
            clearSingleTrack(ite);
        }
        else//增加新的批号
        {
            vectck.push_back( Track( batchNum ) );
            ite = vectck.end()-1;
        }

        ite->append( (int)x, (int)y, angle );
        ite->setAttr(trackattr); //设置属性
        drawSingleTrack(ite);
        //pWarnSectorRegion->warnJudge((int)x, (int)y);

    }
    else
    {
        if( bexit )//批号已经存在,
        {
            ite->append( (int)x, (int)y, angle );
        }
        else//
        {
            vectck.push_back( Track( batchNum ) );
            ite = vectck.end()-1;
            ite->append( (int)x, (int)y, angle );
        }
     }
}

/*!
*    \brief 删除航迹
*
*    \param barchNum    批号
*
*    根据批号删除航迹
*/
void PPISec::delTrack(unsigned int batchNum)
{
    if( dispInfo->bFreeze == true)
            return;
    for( ite = vectck.begin(); ite!=vectck.end(); ite++ )
    {
        if( ite->index() == batchNum )//该批号存在,则准备删除
        {
            clearSingleTrack(ite);
            vectck.erase(ite);
            break;
        }
    }
}

/*!
*    \brief 绘制一条航迹
*
*    \param ite    航迹迭代器
*
*    根据 ite 提供的航迹数据绘制航迹。
*/
void PPISec::drawSingleTrack( VecTrkIte ite )
{
    FB_COLORTYPE color = painter->getColor();
    unsigned int attr = ite->getAttr();
    if(attr == 0x00)//不明
        painter->setColor(FB_SECOND_AMBIGUITY);
    else if(attr == 0x01)//敌方
        painter->setColor(FB_SECOND_ENEMY);
    else if(attr == 0x02)//我方
        painter->setColor(FB_SECOND_MYSELF);
    else if(attr == 0x03)//友方
        painter->setColor(FB_SECOND_COMPANY);
    else
        painter->setColor(FB_SECOND_AMBIGUITY);

    //画航迹点
    DeqIntIte xite,yite;
    for( xite = ite->xbegin(), yite = ite->ybegin(); xite!=ite->xend() && yite!=ite->yend(); xite++, yite++ )
        painter->drawTrackDot( *xite, *yite );

    //画目标批号
    painter->drawPlane( *(ite->xend()-1), *(ite->yend()-1), *(ite->aend()-1), ite->index() );

    painter->setColor(color);
}

/*!
*    \brief 绘制航迹
*/
void PPISec::drawAllTrack()
{
    for( ite = vectck.begin(); ite!=vectck.end(); ite++ )
    {
        drawSingleTrack(ite);
    }
}

/*!
*    \brief 清除一条航迹
*
*    \param ite    航迹迭代器
*/
void PPISec::clearSingleTrack( VecTrkIte ite )
{
    FB_COLORTYPE color = painter->getColor();
    painter->setColor(FB_CLEAR);
    DeqIntIte xite,yite;
    for( xite = ite->xbegin(), yite = ite->ybegin(); xite!=ite->xend() && yite!=ite->yend(); xite++, yite++ )
        painter->drawTrackDot( *xite, *yite );
    painter->drawPlane( *(ite->xend()-1), *(ite->yend()-1), *(ite->aend()-1), ite->index() );
    painter->setColor(color);
}

/*!
*    \brief 清除航迹
*/
void PPISec::clearAllTrack()
{
    for( ite = vectck.begin(); ite!=vectck.end(); ite++ )
    {
        clearSingleTrack(ite);
    }
}

/*!
*    \brief 清除二次信息
*
*    该函数清除 PPI 的二次显示和航迹数据。
*/
void PPISec::clearSurface()
{
    clearAllTrack();
    delAllTrack();
}

//! 清除航迹数据
void PPISec::delAllTrack()
{
    vectck.clear();
}

/*
//设置量程范围
void CPPISec::setRangeWeight(int cont )
{
    int range=500;
    switch(cont)
    {
        case 1:range = 75;break;
        case 2:range = 150;break;
        case 3:range = 300;break;
        case 4:range = 500;break;
        default:range = 500;break;
    }
    if(range != dispInfo->rangePPI)
    {
        //先擦除原来的显示
        MFB_COLORTYPE color = painter->getColor();
        painter->setColor(MFB_CLEAR);
        clearAllTrack();
        delAllTrack();
        painter->drawDistantCircle(dispInfo->centerX,dispInfo->centerY);
        dispInfo->rangePPI = range;
        painter->rangePPI = range;
        pSectorRegion->changeRange(range);
        pWarnSectorRegion->changeRange(range);
        painter->setColor(color);
    }
}
//设置距离圈
void CPPISec::setDistantNum(int cont )
{
    int num=1;
    switch(cont)
    {
        case 1:num = 0;break;
        case 2:num = 1;break;
        case 3:num = 3;break;
        case 4:num = 7;break;
        case 5:num = 9;break;
        default:num = 1;break;
    }
    //先擦除原来的显示
    MFB_COLORTYPE color = painter->getColor();
    painter->setColor(MFB_CLEAR);
    painter->drawDistantCircle(dispInfo->centerX,dispInfo->centerY);
    painter->drawCircle(dispInfo->centerX,dispInfo->centerY,dispInfo->radiusPPI);
    painter->distantCircleCount = num;
    painter->setColor(color);
}
//设置半径
void CPPISec::setRadius(int cont)
{
    dispInfo->radiusPPI = cont;
    painter->radiusPPI = cont;
    pSectorRegion->changeRadius(cont);
    pWarnSectorRegion->changeRadius(cont);
}

//设置圆心
void CPPISec::setCenter(int cx,int cy)
{
     dispInfo->centerX = cx;
     dispInfo->centerY = cy;
     pSectorRegion->setCenter(cx,cy);
     pWarnSectorRegion->setCenter(cx,cy);
}
*/

/*
//响应鼠标按键消息
int CPPISec::mousePress(QMouseEvent* e)
{
    int cx = e->x();
    int cy = e->y();
    if(dispInfo->enadivert == 1 && dispInfo->flagdivert==0)
    {
        //在偏心之前清除二次画面
        MFB_COLORTYPE color = painter->getColor();

        clearAllTrack();
        delAllTrack();
        painter->setColor(MFB_CLEAR);
        painter->drawDistantCircle(dispInfo->centerX,dispInfo->centerY);
        painter->drawCircle(dispInfo->centerX,dispInfo->centerY,dispInfo->radiusPPI);
        painter->drawCrossLine(dispInfo->centerX,dispInfo->centerY,dispInfo->radiusPPI);
        painter->drawScale(dispInfo->centerX,dispInfo->centerY);
        //清除舰首线
        painter->drawLine(dispInfo->centerX,dispInfo->centerY,headAzi,dispInfo->radiusPPI);
        pSectorRegion->clearAllSectorRegion();
        pWarnSectorRegion->clearAllSectorRegion();
        //设置半径
        setRadius(PPI_RADIUS * 2);//512*2
        //设置圆心坐标
        setCenter(cx,cy);
        //更新数据
        painter->getScaleCircle();
        dispInfo->flagdivert=1;

        painter->setColor(color);
    }
    else if(pSectorRegion->regionEna)
    {
        int x,y;
        x= cx - dispInfo->centerX;
        y= dispInfo->centerY - cy;
        if(e->button() == Qt::LeftButton)
            pSectorRegion->setPressPoint(1,x,y);
        else if(e->button() == Qt::RightButton)
            pSectorRegion->setPressPoint(0,x,y);
    }
    else if(pWarnSectorRegion->regionEna)
    {
        int x,y;
        x= cx - dispInfo->centerX;
        y= dispInfo->centerY - cy;
        if(e->button() == Qt::LeftButton)
            pWarnSectorRegion->setPressPoint(1,x,y);
        else if(e->button() == Qt::RightButton)
            pWarnSectorRegion->setPressPoint(0,x,y);
    }
    else
        return(0);
    return(1);
}
//响应鼠标移动消息
void CPPISec::mouseMove(int cx,int cy)
{
     if(pSectorRegion->regionEna)
     {
        int x,y;
        x= cx - dispInfo->centerX;
        y= dispInfo->centerY - cy;
        pSectorRegion->setMovePoint(x,y);
     }
     else if(pWarnSectorRegion->regionEna)
     {
        int x,y;
        x= cx - dispInfo->centerX;
        y= dispInfo->centerY - cy;
        pWarnSectorRegion->setMovePoint(x,y);
     }
}
*/

/*
//设置挖区工作方式
void CPPISec::setSectorRegion(int flag)
{
     pSectorRegion->setSectorRegion(flag);
}
//挖区设置成功,将参数传递给监控
void CPPISec::sendSectorRegion(sectorRegionInfo *info)
{
    unsigned char *buf = new unsigned char[10];
    double scale = (double)dispInfo->rangePPI/(double)dispInfo->radiusPPI;
    //double scale = (double)dispInfo->rangePPI/656.0;//信号处理协议
    //挖区标志及序号
    buf[0] = info->flag + info->id;

    //起始方位高四位
    buf[1] = (info->degree1 >> 8) & 0x000f;
    //起始方位低八位
    buf[2] = (info->degree1) & 0x00ff;
    //起始距离高两位
    double djl = (double)(info->r1)*scale;
    if(djl>5.0)
        djl = (djl-5.0) / 0.75;//0.76219 = (500/656);
    else
        djl = 0.0;
    int ijl = (int)djl;
    buf[3] = (ijl >> 8) & 0x0003;
    //起始距离低八位
    buf[4] = ijl & 0x00ff;

    //终止方位高四位
    buf[5] = (info->degree2 >> 8) & 0x000f;
    //终止方位低八位
    buf[6] = (info->degree2) & 0x00ff;

    //终止距离高两位
    djl = (double)(info->r2)*scale;
    if(djl>5.0)
        djl = (djl-5.0) / 0.75;//0.76219 = (500/656);
    else
        djl = 0.0;
    ijl = (int)djl;
    buf[7] = (ijl >> 8) & 0x0003;
    //终止距离低八位
    buf[8] = (ijl) & 0x00ff;

    emit(setSectorRegionInfo(buf));
    delete []buf;
}
//设置警戒区
void CPPISec::setWarnSectorRegion(int flag)
{
     pWarnSectorRegion->setSectorRegion(flag);
}
//使能偏心显示
void CPPISec::enableDivert()
{
     dispInfo->enadivert = 1;

}
//禁止偏心显示
 void CPPISec::disableDivert()
{

     MFB_COLORTYPE color = painter->getColor();
     dispInfo->enadivert = 0;
     dispInfo->flagdivert = 0;
     clearAllTrack();
     delAllTrack();
     painter->setColor(MFB_CLEAR);
     painter->drawDistantCircle(dispInfo->centerX,dispInfo->centerY);
     painter->drawCircle(dispInfo->centerX,dispInfo->centerY,dispInfo->radiusPPI);
     painter->drawCrossLine(dispInfo->centerX,dispInfo->centerY,dispInfo->radiusPPI);
     painter->drawScale(dispInfo->centerX,dispInfo->centerY);
     //清除舰首线
     painter->drawLine(dispInfo->centerX,dispInfo->centerY,headAzi,dispInfo->radiusPPI);
     pSectorRegion->clearAllSectorRegion();
     pWarnSectorRegion->clearAllSectorRegion();

     setRadius(PPI_RADIUS);
     setCenter(MFB_WIDTH/2,MFB_HEIGHT/2);
     painter->getScaleCircle();

     painter->setColor(color);

}
//使能二次显示
void CPPISec::enableSecondInfo()
{
     dispInfo->bSecondInfo = 1;
}
//禁止二次显示
void CPPISec::disableSecondInfo()
{
     dispInfo->bSecondInfo = 0;
     clearSurface();
    // clearAllTrack();
}
//画面冻结
void CPPISec::freezeSecondInfo(int flag)
{
    if(flag == 0)
    {
        dispInfo->bFreeze = false;
        clearAllTrack();
        delAllTrack();
     }
     else
     {
         dispInfo->bFreeze = true;
      }
}

void CPPISec::drawHead(int azi)
{
    MFB_COLORTYPE color = painter->getColor();
    painter->setColor(MFB_CLEAR);
    painter->drawLine(dispInfo->centerX,dispInfo->centerY,headAzi,dispInfo->radiusPPI);
    headAzi = azi;
    painter->setColor(color);
}
*/
