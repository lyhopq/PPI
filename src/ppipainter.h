#ifndef _PAINTER_H_
#define _PAINTER_H_

#include "def.h"
#include "datapool.h"

/*!
*    \class PPIPainter
*    \brief 在 FrameBuffer 上绘制元素
*
*    提供在 FrameBuffer 上绘制 PPI 元素的函数。例如：画线 \a drawLine()、画圆 \a drawCircle()等等。
*/

class PPIPainter
{
public:
    PPIPainter( DataPool *dp );
    ~PPIPainter();

    inline bool isValid( int x, int y );
    inline void setPixel(int x, int y, FB_COLORTYPE color);

    void drawRect(int left, int up, int right, int down, FB_COLORTYPE color);
    int drawText( int x, int y, int qh, int wh, FB_COLORTYPE color);
    //void drawArc(int deg1, int deg2, int r, FB_COLORTYPE color);

    ///////////////////////////////////
    void drawCircle(int x,int y,int r);
    void drawDistantCircle(int x,int y);
    void drawPoint(int x,int y,int code);
    void drawNum(int x,int y,int num);
    void drawScale(int x,int y);
    void drawCrossLine(int x,int y,int r);
    void getScaleCircle();
    void getDistantCircle();
    //void drawLine(int x,int y,int angle,int len);
    void drawTrackDot(int x,int y);
    //void drawPlane(int x,int y,int angle,int batchNum);
    //void drawSectorRegion(int centerX,int centerY,int r1,int degree1,int r2,int degree2);
    //void drawSectorRegion(int centerX,int centerY,int r1,int degree1,int r2,int degree2,int idNum);
    //void getAddrXY(int radius);

    void setColor(FB_COLORTYPE col){color = col;}
    FB_COLORTYPE getColor(){return color;}
    //////////////////////////////////

private:
    DataPool *m_dp;
    int ** nx;
    int ** ny;
    int ** fx;
    int ** fy;
    //FB_COLORTYPE *colortable;
    FB_COLORTYPE ** fbp;//pointer to framebuffer mem
    //s_layer_unit ** layer;//frame buffer layer

    FB_COLORTYPE color;
    int **chartScaleX,**chartScaleY;
    int **chartAddrX,**chartAddrY;
    int **chartCircle;
    int radiusPPI;
    int rangePPI;//量程
    int textTransFlag;
    int distantCircleCount;
    int centerX,centerY;
    char dot[12][16];//
};

#endif
