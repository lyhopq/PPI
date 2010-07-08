#include "ppipainter.h"
#include "sysvalue.h"

#include <QDebug>

extern sysValue *sysval;

/*!
*    \brief 构造函数
*
*    \param dp DataPool 指针
*
*    加载数据表指针，初始化数字码
*/
PPIPainter::PPIPainter( DataPool *dp )
{
    m_dp = dp;
    nx = m_dp->addr_near_x;
    ny = m_dp->addr_near_y;
    fx = m_dp->addr_far_x;
    fy = m_dp->addr_far_y;
    colortable = m_dp->colortable;
    fbp = m_dp->fbp;
    layer = m_dp->layer;

    distantCircleCount = 1;

    //chartAddrX = NULL;
    //chartAddrY = NULL;
    chartScaleX = NULL;
    chartScaleY = NULL;

    /*
    radiusPPI = PPI_R/2;
    rangePPI = 500;
    centerX = FB_WIDTH/2;
    centerY = FB_HEIGHT/2;
    */

    textTransFlag  = 1;
    color = FB_GREEN;
    //getAddrXY(1024);
    getScaleCircle();

    int i=0;//0
    dot[i][0] = 0x00;dot[i][1] = 0x00;dot[i][2] = 0x00;dot[i][3] = 0x3c;
    dot[i][4] = 0x42;dot[i][5] = 0x42;dot[i][6] = 0x42;dot[i][7] = 0x42;
    dot[i][8] = 0x42;dot[i][9] = 0x42;dot[i][10] = 0x42;dot[i][11] = 0x42;
    dot[i][12] = 0x3c;dot[i][13] = 0x00;dot[i][14] = 0x00;dot[i][15] = 0x00;
    i++;//1
    dot[i][0] = 0x00;dot[i][1] = 0x00;dot[i][2] = 0x00;dot[i][3] = 0x08;
    dot[i][4] = 0x18;dot[i][5] = 0x28;dot[i][6] = 0x08;dot[i][7] = 0x08;
    dot[i][8] = 0x08;dot[i][9] = 0x08;dot[i][10] = 0x08;dot[i][11] = 0x08;
    dot[i][12] = 0x08;dot[i][13] = 0x00;dot[i][14] = 0x00;dot[i][15] = 0x00;
    i++;//2
    dot[i][0] = 0x00;dot[i][1] = 0x00;dot[i][2] = 0x00;dot[i][3] = 0x3c;
    dot[i][4] = 0x42;dot[i][5] = 0x42;dot[i][6] = 0x02;dot[i][7] = 0x02;
    dot[i][8] = 0x04;dot[i][9] = 0x08;dot[i][10] = 0x10;dot[i][11] = 0x20;
    dot[i][12] = 0x7e;dot[i][13] = 0x00;dot[i][14] = 0x00;dot[i][15] = 0x00;
    i++;//3
    dot[i][0] = 0x00;dot[i][1] = 0x00;dot[i][2] = 0x00;dot[i][3] = 0x3c;
    dot[i][4] = 0x42;dot[i][5] = 0x42;dot[i][6] = 0x02;dot[i][7] = 0x1c;
    dot[i][8] = 0x02;dot[i][9] = 0x02;dot[i][10] = 0x42;dot[i][11] = 0x42;
    dot[i][12] = 0x3c;dot[i][13] = 0x00;dot[i][14] = 0x00;dot[i][15] = 0x00;
    i++;//4
    dot[i][0] = 0x00;dot[i][1] = 0x00;dot[i][2] = 0x00;dot[i][3] = 0x04;
    dot[i][4] = 0x0c;dot[i][5] = 0x14;dot[i][6] = 0x24;dot[i][7] = 0x24;
    dot[i][8] = 0x44;dot[i][9] = 0x84;dot[i][10] = 0xfe;dot[i][11] = 0x04;
    dot[i][12] = 0x04;dot[i][13] = 0x00;dot[i][14] = 0x00;dot[i][15] = 0x00;
    i++;//5
    dot[i][0] = 0x00;dot[i][1] = 0x00;dot[i][2] = 0x00;dot[i][3] = 0x3e;
    dot[i][4] = 0x20;dot[i][5] = 0x40;dot[i][6] = 0x7c;dot[i][7] = 0x42;
    dot[i][8] = 0x02;dot[i][9] = 0x02;dot[i][10] = 0x42;dot[i][11] = 0x44;
    dot[i][12] = 0x3c;dot[i][13] = 0x00;dot[i][14] = 0x00;dot[i][15] = 0x00;
    i++;//6
    dot[i][0] = 0x00;dot[i][1] = 0x00;dot[i][2] = 0x00;dot[i][3] = 0x1c;
    dot[i][4] = 0x22;dot[i][5] = 0x40;dot[i][6] = 0x40;dot[i][7] = 0x5c;
    dot[i][8] = 0x42;dot[i][9] = 0x42;dot[i][10] = 0x42;dot[i][11] = 0x42;
    dot[i][12] = 0x3c;dot[i][13] = 0x00;dot[i][14] = 0x00;dot[i][15] = 0x00;
    i++;//7
    dot[i][0] = 0x00;dot[i][1] = 0x00;dot[i][2] = 0x00;dot[i][3] = 0x7e;
    dot[i][4] = 0x02;dot[i][5] = 0x04;dot[i][6] = 0x04;dot[i][7] = 0x08;
    dot[i][8] = 0x08;dot[i][9] = 0x08;dot[i][10] = 0x10;dot[i][11] = 0x10;
    dot[i][12] = 0x10;dot[i][13] = 0x00;dot[i][14] = 0x00;dot[i][15] = 0x00;
    i++;//8
    dot[i][0] = 0x00;dot[i][1] = 0x00;dot[i][2] = 0x00;dot[i][3] = 0x3c;
    dot[i][4] = 0x42;dot[i][5] = 0x42;dot[i][6] = 0x42;dot[i][7] = 0x3c;
    dot[i][8] = 0x42;dot[i][9] = 0x42;dot[i][10] = 0x42;dot[i][11] = 0x42;
    dot[i][12] = 0x3c;dot[i][13] = 0x00;dot[i][14] = 0x00;dot[i][15] = 0x00;
    i++;//9
    dot[i][0] = 0x00;dot[i][1] = 0x00;dot[i][2] = 0x00;dot[i][3] = 0x3c;
    dot[i][4] = 0x42;dot[i][5] = 0x42;dot[i][6] = 0x42;dot[i][7] = 0x46;
    dot[i][8] = 0x3a;dot[i][9] = 0x02;dot[i][10] = 0x02;dot[i][11] = 0x44;
    dot[i][12] = 0x38;dot[i][13] = 0x00;dot[i][14] = 0x00;dot[i][15] = 0x00;

    //点迹点
    dot[10][0] = 0x0e;
    dot[10][1] = 0x1f;
    dot[10][2] = 0x1f;
    dot[10][3] = 0x1f;
    dot[10][4] = 0x0e;

    /*
    dot[10][0] = 0x00;
    dot[10][1] = 0x0e;
    dot[10][2] = 0x0e;
    dot[10][3] = 0x0e;
    dot[10][4] = 0x00;

    dot[10][5] = 0x00;
    dot[10][6] = 0x00;
    dot[10][7] = 0x00;
    */

}

/*!
*    \brief 析构函数
*
*    删除所申请的内存
*/
PPIPainter::~PPIPainter()
{
    int i;

    /*
    for(i=0;i<AZI_NUM;i++)
          delete[] chartAddrX[i];
    if(chartAddrX!=NULL) delete[] chartAddrX;

    for(i=0;i<AZI_NUM;i++)
          delete[] chartAddrY[i];
    if(chartAddrY!=NULL) delete[] chartAddrY;
    */

    for(i=0; i<90+1;i++)
    {
        delete[] chartScaleX[i];
        delete[] chartScaleY[i];
    }

    if(chartScaleX != NULL) delete[] chartScaleX;
    if(chartScaleY != NULL) delete[] chartScaleY;

}

/*
bool PPIPainter::isValid( int x, int y )
{
    return (x>=0) && (x<FB_WIDTH) && (y>=0) && (y<FB_HEIGHT);
}
*/

/*!
*    \brief 画点
*
*    \param x 屏幕x坐标
*    \param y 屏幕y坐标
*    \param color 颜色
*
*    在屏幕像素点 (x,y) 处画点，颜色为 color
*/
void PPIPainter::setPixel(int x, int y, FB_COLORTYPE color)
{
    //if( isValid( x, y ) )
    {
        if(color != FB_CLEAR)
        {
            layer[y][x].bsecond = true;
            if( !( layer[y][x].bmask ) )
             {
                fbp[y][x] = color;
             }
        }
        else
        {
            layer[y][x].bsecond = false;
            if( !( layer[y][x].bmask ) )
             {
               fbp[y][x] = colortable[layer[y][x].nfirst];
             }
        }
    }
}

/*!
*    \brief 画矩形
*
*    \param letf 左上角x坐标
*    \param up 左上角y坐标
*    \param right 右下角x坐标
*    \param dowm 右下角y坐标
*    \param color 颜色
*
*    在屏幕上画矩形，左上角坐标为 (left, up)，右下角坐标为 (right, down)。颜色为 color。
*/
void PPIPainter::drawRect(int left, int up, int right, int down, FB_COLORTYPE color)
{
    for(int y=up;y>down;y--)
    {
        for(int x=left;x<right;x++)
        {
            setPixel(x,y,color);
        }
    }
}

/*!
*    \brief 画直线
*
*    \param x    起点x坐标
*    \param y    起点y坐标
*    \param angle 倾角
*    \param len  长度
*
*    以 (x,y) 为起点， angle 为倾角， 绘制一条长度为 len 的直线
*/
void PPIPainter::drawLine(int x,int y,int angle,int len)
{
    int i,px,py;
    for(i=0;i<len;i++)
    {
        px = nx[angle][i];
        py = ny[angle][i];
        setPixel(x+px,y-py,color);
    }
}

/*!
*    \brief 画坐标十字坐标
*
*    \param x 坐标中心x值
*    \param y 坐标中心y值
*    \param r 十字坐标半径
*
*    在屏幕 (x,y) 处画十字坐标，半径为r。
*/
void PPIPainter::drawCrossLine(int x,int y,int r)
{
    for(int i=-r;i<r;i++)
    {
        setPixel(x+i,y,color);
        setPixel(x,y-i,color);
    }
}

/*!
*    画阿拉伯数字 0～9
*/
void PPIPainter::drawPoint(int x,int y,int code)
{
    int i=0,j;
    unsigned int shift;
    if(textTransFlag == 0) // 透明？？？？
    {
        for(i=0; i<16; i++)
        {
            shift=0x100;
            for(j=0;j<8;j++)
            {
                shift = shift>>1;
                if(dot[code][i] & shift)
                {
                    setPixel( x+j , y+i , color );
                }
            }
        }
    }
    else
    {
        for(i=0; i<16; i++)
        {
            shift=0x100;
            for(j=0;j<8;j++)
            {
                shift = shift>>1;
                if(dot[code][i] & shift)
                    setPixel( x+j , y+i , color );
                else
                    setPixel( x+j , y+i , FB_CLEAR );
            }
        }
    }
}

/*!
*    \brief 在屏幕上画数字
*
*    \param x x坐标
*    \param y y坐标
*    \param num 数字
*
*    在屏幕 (x,y) 处画数字 num
*
*    \warning 数字位数不能超过 10 位。
*/
void PPIPainter::drawNum(int x,int y,int num)
{
    int i=0,j=0;
    unsigned char n[10];
    do{
        n[i] = num%10;
        num  = num/10;
        if(num == 0)
            break;
        i++;
    }while(1);
    for(j=i;j>=0;j--)
        drawPoint(x + (i-j)*8,y,n[j]);
}

/*!
*    \brief 画园
*
*    \param x    圆心x坐标
*    \param y    圆心y坐标
*    \param r    半径
*
*    在屏幕上以 (x,y) 为圆心，r为半径画圆。采用查表的方法速度提高。
*/
void PPIPainter::drawCircle(int x,int y,int r)
{
    int px,py;
    int ** pnx = nx;
    int ** pny = ny;
    //int scale = 1024;//11.378=360.0*4096.0;
    int scale = 1024;//11.378=360.0*4096.0;
    if(r > PPI_r)
    {
        pnx = fy;
        pny = fx;
        scale = 2048;
    }

    int angle=0;
    for(angle=0;angle<scale;angle++) // 根据圆的对称型，减少循环次数
    {
        px = pnx[angle][r-1];
        py = pny[angle][r-1];

        setPixel(x+px,y+py,color);
        setPixel(x+px,y-py,color);
        setPixel(x-px,y+py,color);
        setPixel(x-px,y-py,color);
        setPixel(x+py,y+px,color);
        setPixel(x+py,y-px,color);
        setPixel(x-py,y+px,color);
        setPixel(x-py,y-px,color);
    }
}


/*!
*    \brief 画距离圈
*
*    \param x    圆心x坐标
*    \param y    圆心y坐标
*
*    在屏幕上以 (x,y) 为圆心画距离圈。距离圈的个数取决于 \a distantCircleCount。
*    平且在距离圈上标注该距离圈所表示的距离(km)。
*/
void PPIPainter::drawDistantCircle(int x,int y)
{
    int px,py;
    int R = (int)((double)sysval->getRadius()/(double)(distantCircleCount+1));
    double num = (double)sysval->getRange()/(double)(distantCircleCount+1);

    int ** pnx = nx;
    int ** pny = ny;
    int scale = 1024;//11.378=360.0*4096.0;
    for(int i=1;i<=distantCircleCount ;i++)
    {
        int r = R*i;
        int angle=0;
        pnx = ny;
        pny = nx;
        scale = 1024;
        if(r > PPI_r)
        {
            pnx = fy;
            pny = fx;
            scale = 2048;
        }
        for(angle=0;angle<scale;angle++)
        {
            px = pnx[angle][r-1];
            py = pny[angle][r-1];
            setPixel(x+px,y+py,color);
            setPixel(x+px,y-py,color);
            setPixel(x-px,y+py,color);
            setPixel(x-px,y-py,color);
            setPixel(x+py,y+px,color);
            setPixel(x+py,y-px,color);
            setPixel(x-py,y+px,color);
            setPixel(x-py,y-px,color);
        }
        double jl = num*(double)i;
        drawNum(x+4,y-r-16,(int)jl);
    }
}

/*
void PPIPainter::getAddrXY(int radius)
{
   int i,j;
   long R;
   R = 4096;
   if(chartAddrX != NULL)
       delete[] chartAddrX;
   if(chartAddrY != NULL)
       delete[] chartAddrY;
   chartAddrX=new int*[R+2];
   for(i=0;i<R;i++)
       chartAddrX[i]=new int[radius+2];
   chartAddrY=new int*[R+2];
   for(i=0;i<R;i++)
       chartAddrY[i]=new int[radius+2];
   for(i=0;i<R;i++)
   {
       double angle=(2*3.14159*i) / (double)R;
       for(j=0;j<radius;j++)
       {
           chartAddrX[i][j] = (int)((double)j*sin(angle));
           chartAddrY[i][j] = (int)((double)j*cos(angle));
       }
   }
}
*/

//! 计算画刻度线的数据
void PPIPainter::getScaleCircle()
{
    int i=0,j=0;
    if(chartScaleX != NULL)
       delete[] chartScaleX;
    if(chartScaleY != NULL)
       delete[] chartScaleY;
    chartScaleX = new int*[90+1];
    chartScaleY = new int*[90+1];
    for(i=0; i<90+1;i++)
    {
        chartScaleX[i] = new int[12+2];
        chartScaleY[i] = new int[12+2];
    }
    int angle=0;
    int ** pnx = nx;
    int ** pny = ny;
    double scale = 11.378;//11.378*360.0=4096.0;
    if(sysval->getRadius() > PPI_r)
    {
        pnx = fy;
        pny = fx;
        scale = 22.756;
    }
    for(angle=0;angle<90+1;angle++)
    {
        int a = (int)((double)angle*scale);
        for(j=0;j<12;j++)
        {

            chartScaleX[angle][j] = pnx[a][sysval->getRadius()-j];
            chartScaleY[angle][j] = pny[a][sysval->getRadius()-j];
        }
        chartScaleX[angle][j] = pnx[a][sysval->getRadius()-24]; // 刻度值的起始坐标
        chartScaleY[angle][j] = pny[a][sysval->getRadius()-24];
    }
}

/*!
*    \brief 画刻度
*
*    \param x    圆心x坐标
*    \param y    圆心y坐标
*
*    画极坐标的刻度，并标上角度。
*/
void PPIPainter::drawScale(int x,int y)
{
    int px,py,i,j,len,shift;
    for(i=0;i<=90;i++)//0-90度
    {
        shift = (int)((double)i*11.38);
        if(i%5 == 0)//每5度显示一长刻度
        {
            len = 12;
            if(i%15 == 0)//每15度显示度数
            {
                px = chartScaleX[i][12];
                py = chartScaleY[i][12];
                drawNum(x+px-4,y-py-8,i);//0-90
                if(180-i<100)//90-180
                    drawNum(x+px-4,y+py-8,180-i);
                else
                    drawNum(x+px-12,y+py-8,180-i);
                drawNum(x-px-12,y+py-8,i+180);//180-270
                if( i!=0 )
                    drawNum(x-px-12,y-py-8,360-i);//270-360
            }
        }
        else
            len = 7;
        for(j=0;j<len;j++)
        {
            px = chartScaleX[i][j];
            py = chartScaleY[i][j];
            setPixel(x+px,y+py,color);
            setPixel(x+px,y-py,color);
            setPixel(x-px,y+py,color);
            setPixel(x-px,y-py,color);
        }
    }
}


/*!
*    \brief 画航迹点
*
*    \param x    x坐标
*    \param y    y坐标
*
*    在屏幕 (x,y) 处绘制一个航迹点。航迹点的数据为 dot[10]
*/
void PPIPainter::drawTrackDot(int x,int y)
 {
    int i=0,j,shift;
    for(i=-2; i<3; i++)
    {
        shift=0x01;
        for(j=-2;j<3;j++)
        {
            if(dot[10][i+2] & shift)
            {
                setPixel(x+j,y+i,color);
            }
            shift = shift<<1;
        }
    }
 }

#define A360  4096
#define A45   512
#define A90   1024
#define A150  1707
#define A180  2048
#define A210  2389
#define A270  3072

void PPIPainter::drawPlane(int x,int y,int angle,int batchNum)
{
    //int px = chartAddrX[angle][14];
    //int py = chartAddrY[angle][14];
    int px = nx[angle][14];
    int py = ny[angle][14];
//     if(angle < A45)
//     {
//       px = chartAddrX[angle+A150][10] ;
//       py = chartAddrY[angle+A150][10] ;
//     }
//     else if(angle < A45*2)
//     {
//         px = chartAddrX[angle+A150][10];
//         py = chartAddrY[angle+A150][10];
//     }
//     else if(angle < A45*3)
//     {
//         px = chartAddrX[angle+A150][10];
//         py = chartAddrY[angle+A150][10];
//     }
//     else if(angle < A45*4)
//     {
//         px = chartAddrX[angle+A150][20];
//         py = chartAddrY[angle+A150][20];
//     }
//     else if(angle < A45*5)
//     {
//         px = chartAddrX[angle+A90][20];
//         py = chartAddrY[angle+A90][20];
//     }
//     else if(angle < A270)
//     {
//         px = chartAddrX[angle-A150][20];
//         py = chartAddrY[angle-A150][20];
//     }
//     else if(angle < A45*7) //270-315
//     {
//         px = chartAddrX[angle-A150][20];
//         py = chartAddrY[angle-A150][20];
//
//     }
//      else if(angle < A45*8) //315-360
//     {
//         px = chartAddrX[angle-A150][20];
//         py = chartAddrY[angle-A150][20];
//     }
    if(angle < A45)
    {
      px = nx[angle][38] - 4;
      py = ny[angle][38] - 4;
    }
    if(angle < A45*2)
    {
        px = nx[angle][24];
        py = ny[angle][38];
    }
    else if(angle < A180)
    {
        px = nx[angle][22] - 8;
        py = ny[angle][22];
    }
    else if(angle < A270)
    {
        px = nx[angle][22] - 8 ;
        py = ny[angle][22];
    }
    else if(angle < A45*7) //270-315
    {
        px = nx[angle][38] + 8;
        py = ny[angle][38] + 8;

    }
     else if(angle < A45*8) //315-360
    {
        px = nx[angle][38] - 4;
        py = ny[angle][38];
    }

    int oldflag = textTransFlag;
    textTransFlag = 0;

    int dx = x+px;
    int dy = y-px;
    //qDebug() << dx << dy;
    if((dx < FB_WIDTH - 50) && (dx > 50) && (dy < FB_HEIGHT - 50) && (dy > 50))
    {
        drawLine(x,y,angle,25);
        drawNum(x+px,y-py,batchNum);
    }
    textTransFlag = oldflag;
}


void PPIPainter::drawSectorRegion(int centerX,int centerY,int r1,int degree1,int r2,int degree2)
{
    int **chartAddrX,**chartAddrY;
    chartAddrX = nx; chartAddrY = ny;
    int start,diff,i,x,y;
    if(r1>r2)
    {
        start = r2; // 外径
        diff = r1-r2; // 间距
    }
    else
    {
        start = r1;
        diff = r2-r1;
    }
    //qDebug() << diff << "!!!!!!!!!!!!!!!!!!!!!!!";
    for(i=0;i<diff;i++)
    {
        x = centerX + chartAddrX[degree1][start + i];
        y = centerY - chartAddrY[degree1][start + i];
        setPixel(x,y,color);
        //qDebug() << "(" << x << y << ")";
        //printf("(%d, %d)\n", x, y);
        x = centerX + chartAddrX[degree2][start + i];
        y = centerY - chartAddrY[degree2][start + i];
        setPixel(x,y,color);
    }
    if(degree1>degree2)
        diff = (degree2-degree1)+4096;
    else
        diff = degree2-degree1;
    for(i=0; i<diff;i++)
    {
        int degree = (degree1+i)%4096;
        x = centerX + chartAddrX[degree][r1];
        y = centerY - chartAddrY[degree][r1];
        setPixel(x,y,color);
        x = centerX + chartAddrX[degree][r2];
        y = centerY - chartAddrY[degree][r2];
        setPixel(x,y,color);
    }
}

void PPIPainter::drawSectorRegion(int centerX,int centerY,int r1,int degree1,int r2,int degree2,int idNum)
{
    if(r1 > sysval->getRadius())
        r1 = sysval->getRadius();

    if(r2 > sysval->getRadius())
        r2 = sysval->getRadius();

    if((r1 == r2) && (r1 == sysval->getRadius()))
        return;

    int r = r2>r1?r2:r1; // r 外径

    drawSectorRegion(sysval->getCenterX(), sysval->getCenterY(),r1,degree1,r2,degree2);

    if(r>10)
        r-=10;
    int px =  nx[degree2][r];
    int py =  ny[degree2][r];
    //drawNum(sysval->getCenterX() + px, sysval->getCenterY() - py,idNum);
    if(r1 != r2)
        drawNum(centerX + px, centerY - py,idNum);
}

/*
/////////////////////////////////////////////
//根据输入的区位码,输出汉字
//qh区号,wh位号
int CPainter::drawText( int x, int y, int qh, int wh, MFB_COLORTYPE color)
{
    int location;
    char buffer[32];
    FILE *fp;

    // read font info from font lib file
    location=(94*(qh-1)+(wh-1))*32;

    if((fp=fopen(MFB_FONTFILE,"rb"))==NULL)
    {
        printf("font file cannot open\n");
        return -1;
    }
    fseek( fp, location, 0 );
    fread( buffer, 32, 1, fp);

    // drawing on the framebuffer surface
    for(int i=0;i<16;i++)
        for(int j=0;j<2;j++)
            for(int k=0;k<8;k++)
                if(buffer[i*2+j]>>(7-k)&0x1)
                    setPixel(x+8*j+k,y-i,color);

    fclose(fp);
    return 0;
}
//需要输入地址映射表,用查表的方法避免死地址
void CPainter::drawArc(int deg1, int deg2, int r, MFB_COLORTYPE color)
{
    if(r<512)
    {
        for(int i=deg1;i<deg2;i++)
        {
            setPixel( *(nx[i]+r), *(ny[i]+r), color );
        }
    }
    else
    {
        r=r&0x1ff;
        for(int i=deg1;i<2*deg2;i++)
        {
            setPixel( *(fx[i]+r), *(fy[i]+r), color );
        }
    }
}
*/
