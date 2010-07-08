
#include "sectorregion.h"
#include "math.h"
#include "ppipainter.h"
#include "ppisec.h"

#include <QDebug>

extern sysValue *sysval;

SectorRegion::SectorRegion(PPIPainter *p, int maxrgnums, FB_COLORTYPE col)
{
    regionNums = 0;
    firstflag=false;
    regionEna = false;
    painter = p;
    color = col;
    //sectorlist.setAutoDelete ( true);
    maxSectorRegions = maxrgnums;

    /*
    radiusPPI = PPI_r;
    rangePPI = PPI_RANG;
    centerX = MFB_WIDTH/2;
    centerY = MFB_HEIGHT/2;
    */

    regionFlag = 0;//挖除区标志
}

SectorRegion::~SectorRegion()
{
}

//! 打印扇区参数
void SectorRegion::displayRegion(sectorRegionInfo *p)
{
    printf("\n%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%");
    printf("\nID: %d\t Flag: %d\n", p->id, p->flag);
    printf("First Point:\n");
    printf("\t(%d, %d)\n", p->x1, p->y1);
    printf("\t%d\t%d\n", p->r1, p->degree1);
    printf("Second Point:\n");
    printf("\t(%d, %d)\n", p->x2, p->y2);
    printf("\t%d\t%d\n", p->r2, p->degree2);
    printf("%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n");
}

//获取扇形区域分配的编号
int SectorRegion::getSectorRegionId()
{
    int flag=0,i=0,j=0;
    //查找未使用的编号,标号从1到10.
    //qDebug() << sectorlist.size();
    for(i=1;i<10;i++)
    {
        flag = 0;
        //for(psrlist = sectorlist.first();j<sectorlist.size();psrlist = sectorlist.at(++j))
        for(;j<sectorlist.size(); ++j)
        {
            psrlist = sectorlist.at(j);
            if(psrlist->id == i)
            {
               flag = 1;
               break;
            }
        }
        if(flag == 0)
           return(i);
    }
    return(i);
}

//当鼠标移动时,跟踪鼠标作图
void SectorRegion::setMovePoint(int x1,int y1)
{
    if(firstflag == true)
    {
       int r = (int)sqrt(x1*x1 + y1*y1);
       if(r >= sysval->getRadius())//判断是否超出范围
            return;
       double degree = atan2((double)x1 , (double)y1);
       if(degree < 0)
             degree = PI + PI + degree;
       degree = (degree*2048.0) / PI;
       ///保存参数
       psrlist = sectorlist.last();
       if(psrlist)
       {
           clearSigleSectorRegion();
           psrlist->r2 = r;
           psrlist->degree2  = (int)degree;
           drawSigleSectorRegion();
       }
   }
}

//当有鼠标按键时,设置或取消扇形区域的点
void SectorRegion::setPressPoint(int flag,int x1,int y1)
{
    //qDebug() << flag << x1 << y1 << sectorlist.size() << maxSectorRegions;
    if((flag == 0))//按下鼠标右键取消或无效
     {
        if(firstflag == true)//如果已经确定了第一个点,则取消
        {
            clearSigleSectorRegion();
            sectorlist.removeLast();
            firstflag = false;
        }
     }
    else if(flag == 1)//按下鼠标左键确认
    {
        regionNums = sectorlist.size();

        int r;
        double degree;

        //确定扇形区域的第一个点
        if( firstflag == false && (regionNums <= maxSectorRegions-1))
        {
            r = (int)sqrt(x1*x1 + y1*y1);
            if(r >= sysval->getRadius())//判断是否超出范围
                return;
            sectorlist.append(new sectorRegionInfo);
            psrlist = sectorlist.last();
            //计算角度和半径

            degree = atan2((double)x1 , (double)y1);
            if(degree < 0)
                degree = PI + PI + degree;
            degree = (degree*2048.0) / PI;
            //保存参数
            psrlist->r1 = psrlist->r2 = r;
            psrlist->degree1 = psrlist->degree2 = (int)degree;
            psrlist->x1  = x1;
            psrlist->y1  = y1;
            psrlist->id = getSectorRegionId();
            firstflag = true;
        }
        //第二点，生成一个扇形区域
        else if(firstflag == true && (regionNums <= maxSectorRegions))
        {
            firstflag = false;
            psrlist = sectorlist.last();

            r = (int)sqrt(x1*x1 + y1*y1);
            degree = atan2((double)x1 , (double)y1);
            if(degree < 0)
                degree = PI + PI + degree;
            degree = (degree*2048.0) / PI;
            psrlist->r2 = r;
            psrlist->degree2 = degree;

            //统一参数,在r2中保存外径
            if(psrlist->r1 > psrlist->r2)
            {
                int tr = psrlist->r2;
                psrlist->r2 = psrlist->r1;
                psrlist->r1 = tr;
            }
            psrlist->x2 = x1;
            psrlist->y2 = y1;
            psrlist->flag = regionFlag;
           // regionNums++;
            if(regionFlag & 0x30)//内挖区0x10或外挖区0x20
                ;//pOwner->sendSectorRegion(psrlist);
            else//警戒区设置,只有一个区域
                ;//regionEna = false;
        }
     }

     //****************
     //displayRegion(psrlist);
     //****************
}

//设置当前的工作模式
void SectorRegion::setSectorRegion(int flag)
{
    //结束挖区工作模式
   if(flag == 0 || flag == 29 || flag == 30)
   {
       firstflag = false;
       regionEna = false;
   }
   //允许挖区工作
   else if(flag == 1 || flag == 2)
   {
        if(flag == 1)//内挖区
            regionFlag = 0x10;
        else
            regionFlag = 0x20;
       regionEna = true;
   }
   //删除所有挖区
   else if(flag == 4)
   {
       regionNums = 0;
       clearAllSectorRegion();
       qDeleteAll(sectorlist);
       sectorlist.clear();//删除列表
   }
   else if(flag&0xe0)//删除单个挖区
   {
        flag = flag & 0x0f;
        delSigleSectorRegion(flag);
   }
}

void SectorRegion::cancel()
{
    firstflag = false;
    regionEna = false;
}

/*
//设置中心点的坐标
void SectorRegion::setCenter(int cx,int cy)
{
    centerX = cx;
    centerY = cy;
}
*/

//改变半径
void SectorRegion::changeRadius(int radius)
{
    int j=0;
    double scale = double(radius) / (double)sysval->getRadius();
 //   clearAllSectorRegion();
    for(psrlist = sectorlist.first();j<sectorlist.size();psrlist = sectorlist.at(++j))
    {
        psrlist->r1 = (int)((double)(psrlist->r1) * scale+0.5);
        psrlist->r2 = (int)((double)(psrlist->r2) * scale+0.5);
    }
    //this->radiusPPI = radius;
}

//改变量程
void SectorRegion::changeRange(int range)
{
    int j=0;
    double scale =  (double)sysval->getRange()/double(range);
    clearAllSectorRegion();


    for(psrlist = sectorlist.first();j<sectorlist.size();psrlist = sectorlist.at(++j))
    {
        psrlist->r1 = (int)((double)(psrlist->r1) * scale+0.5);
        psrlist->r2 = (int)((double)(psrlist->r2) * scale+0.5);
    }
    //this->rangePPI = range;
}


//删除当前的挖区
void SectorRegion::delSigleSectorRegion(int flag)
{
    int i=0,j=0;
    for(psrlist = sectorlist.first();j<sectorlist.size();psrlist = sectorlist.at(++j))
    {
        if(psrlist->id == flag)
        {
            sectorlist.removeAt(i);
            clearSigleSectorRegion(psrlist);
            break;
        }
        i++;
    }
}

//清除当前的挖区
void SectorRegion::clearSigleSectorRegion(sectorRegionInfo *psrlist)
{
    FB_COLORTYPE col = painter->getColor();
    painter->setColor(FB_CLEAR);
    painter->drawSectorRegion(sysval->getCenterX(),sysval->getCenterY(),psrlist->r1,psrlist->degree1,psrlist->r2,psrlist->degree2,psrlist->id);
    painter->setColor(col);
}

//清除当前的挖区
void SectorRegion::clearSigleSectorRegion()
{
    FB_COLORTYPE col = painter->getColor();
    painter->setColor(FB_CLEAR);
    psrlist = sectorlist.last();
    painter->drawSectorRegion(sysval->getCenterX(),sysval->getCenterY(),psrlist->r1,psrlist->degree1,psrlist->r2,psrlist->degree2,psrlist->id);
    painter->setColor(col);
}
//画当前的挖区
void SectorRegion::drawSigleSectorRegion()
{
    FB_COLORTYPE col = painter->getColor();
    painter->setColor(color);
    psrlist = sectorlist.last();
    painter->drawSectorRegion(sysval->getCenterX(),sysval->getCenterY(),psrlist->r1,psrlist->degree1,psrlist->r2,psrlist->degree2,psrlist->id);
    painter->setColor(col);
}

//画所有的挖区
void SectorRegion::drawAllSectorRegion()
{
    int j=0;
    FB_COLORTYPE col = painter->getColor();
    painter->setColor(color);
    //qDebug() << sectorlist.size();
    //for(psrlist = sectorlist.first();j<sectorlist.size();psrlist = sectorlist.at(++j))
    for(;j<sectorlist.size(); ++j)
    {
        psrlist = sectorlist.at(j);
        painter->drawSectorRegion(sysval->getCenterX(),sysval->getCenterY(),psrlist->r1,psrlist->degree1,psrlist->r2,psrlist->degree2,psrlist->id);
    }
    painter->setColor(col);
}

//清除所有的挖区
void SectorRegion::clearAllSectorRegion()
{
    int j=0;
    FB_COLORTYPE col = painter->getColor();
    painter->setColor(FB_CLEAR);
    for(psrlist = sectorlist.first();j<sectorlist.size();psrlist = sectorlist.at(++j))
    {
       painter->drawSectorRegion(sysval->getCenterX(),sysval->getCenterY(),psrlist->r1,psrlist->degree1,psrlist->r2,psrlist->degree2,psrlist->id);
    }
    painter->setColor(col);
}



WarnSectorRegion::WarnSectorRegion(PPIPainter *p,int maxrgnums,FB_COLORTYPE col)
                  : SectorRegion( p, maxrgnums,col )
{
      warnflag = 0;
      regionFlag = 0x00;
}

void WarnSectorRegion::warnJudge(int x1,int y1)
{
    int j=0;
    x1 = x1 - sysval->getCenterX();
    y1 = sysval->getCenterY() - y1;

    int r = (int)sqrt(x1*x1 + y1*y1);
    double degree = atan2((double)x1 , (double)y1);
    if(degree < 0)
        degree = PI + PI + degree;
    int dg = (int)((degree*2048.0) / PI);
    for(psrlist = sectorlist.first();j<sectorlist.size();psrlist = sectorlist.at(++j))
    {
        if( ((psrlist->r1 >= r) && (psrlist->r2 <= r)) || (psrlist->r1 <= r) && (psrlist->r2 >=r))
        {
            int d2 = psrlist->degree2;
            int d1 = psrlist->degree1;
            if( ((dg>=d1) && (dg<=d2)) || (d2<d1) && ((dg>d1) ||(dg <d2)))
            {
                warnflag = 1;
                break;
            }
        }
    }
}

void WarnSectorRegion::drawAllSectorRegion()
{
    int j=0;
    FB_COLORTYPE col = painter->getColor();
    painter->setColor(color);
    for(psrlist = sectorlist.first();j<sectorlist.size();psrlist = sectorlist.at(++j))
    {
       painter->drawSectorRegion(sysval->getCenterX(),sysval->getCenterY(),psrlist->r1,psrlist->degree1,psrlist->r2,psrlist->degree2,psrlist->id);
    }
    painter->setColor(col);
}

void WarnSectorRegion::setSectorRegion(int flag)
{
   if(flag == 0 )
   {
       firstflag = false;
       regionEna = false;
       warnflag = 0;
       regionNums = 0;
       clearAllSectorRegion();
       qDeleteAll(sectorlist);
       sectorlist.clear();
   }
   else if(flag == 1)
   {
       regionEna = true;
   }
}

int WarnSectorRegion::getWarnFlag()
{
    if(warnflag)
    {
        warnflag = 0;
        return(1);
    }
    else
        return(0);
}
