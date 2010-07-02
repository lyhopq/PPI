#include <string.h>
#include "fblayer.h"

/*!
*    \brief 构造函数
*
*    \param _dp    \a DataPool 指针
*
*    获取数据指针，并初始化偏移为0。
*/
FBLayer::FBLayer(DataPool *_dp)
{
    dp = _dp;
    fbp = dp->fbp;
    layer = dp->layer;

    xoffset = 0;
    yoffset = 0;
}

//! 析构函数
FBLayer::~FBLayer()
{
}


/*!
*    \brief 清除二次数据
*
*    若无二次信息或者屏蔽则清除该点（黑色）
*/
void FBLayer::flushWithSecond()
{
    for(int i=0;i<FB_HEIGHT;i++)
    for(int j=0;j<FB_WIDTH;j++)
    if(!(layer[i][j].bsecond || layer[i][j].bmask ))
        fbp[i][j]=FB_CLEAR;
}

//! 清除一次信息
void FBLayer::clearFirst()
{
    for(int i=0;i<FB_HEIGHT;i++)
    for(int j=0;j<FB_WIDTH;j++)
        layer[i][j].nfirst = 0;
}

//! 清屏
void FBLayer::clear()
{
    for(int i=0;i<FB_HEIGHT;i++)
        memset( fbp[i], 0, FB_WIDTH*sizeof(FB_COLORTYPE) );
}


/*!
*    \brief 设置屏蔽矩形区域
*
*    \param x    矩形左上角x坐标
*    \param y    矩形左上角y坐标
*    \param w    矩形宽度
*    \param h    矩形高度
*    \param b    true (屏蔽)，false (去屏蔽)
*
*    设置左上角坐标为 (x,y)，宽度为 w，高度为 h 的屏蔽区域。
*/
void FBLayer::setMaskRect( int x, int y, int w, int h, bool b)
{
    x = x-xoffset;
    y = y-yoffset;

    for( int i=x; i<x+w; i++ )
    for( int j=y; j<y+h; j++ )
    {
        layer[j][i].bmask = b;
    }
}
