#ifndef _FBLAYER_H_
#define _FBLAYER_H_

#include "def.h"
#include "datapool.h"


/*!
*    \class FBLayer
*    \brief FrameBuffer 层
*
*    对 FrameBuffer 显示进行分层，以便进行多种信息的同步显示。
*/
class FBLayer
{
public:
    FBLayer(DataPool *_dp);
    ~FBLayer();

public:
    //inline bool isValid( int &x, int &y );

    /*!
    *    \brief 设置偏移量
    *
    *    \param xoffset x 偏移
    *    \param yoffset y 偏移
    */
    void setOffset( int x, int y ){ xoffset=x; yoffset=y; }
    void setMaskRect( int x, int y, int w, int h, bool b);
    //void setUnMaskRect( int x, int y, int w, int h );
    void flushWithSecond();
    void clearFirst();
    void clear();

private:
    DataPool *dp;
    FB_COLORTYPE ** fbp;//pointer to framebuffer mem
    s_layer_unit ** layer;//frame buffer layer
    int xoffset, yoffset;
};

#endif
