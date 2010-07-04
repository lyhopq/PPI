#ifndef _PPI_H_
#define _PPI_H_

#include "def.h"
#include "datapool.h"

/*!
*    \class PPI
*    \brief PPI 的一次信息
*
*    提供你 PPI 的一次信息操作接口。如： \a setOrigin()、 \a setRange()、 \a ppiDraw() 等等。
*    调用 \a ppiDraws() 可以在屏幕显示一次信息。
*/
class PPI
{
public:
    PPI( DataPool *dp );
    ~PPI();

public:
    void setOrigin( int mx, int my );
    //! 得到圆心 x 坐标
    int getXorg() const {return xorg;}
    //! 得到圆心 y 坐标
    int getYorg() const {return yorg;}
    void resetOrigin();
    //! 设置视频增益
    void setGain( int g ) { gain = g; }
    //! 获取视频增益
    int getGain(){return gain;}
    //! ??????????
    void setBase( int b ){ base = b; }
    void setRange( int r );
    //! 获取量程
    int getRange() const {return range;}

    //void enableDivert() { bdivert = 1; }
    //void disableDivert() { bdivert = 0; }
    //bool isDivert() { return bdivert; }

    //! 显示一次信息
    void enableFirstInfo(){ bFirstInfo = true; }
    //! 不显示一次信息
    void disableFirstInfo(){ bFirstInfo = false; }
    //! 一次信息是否显示
    bool isFirstInfo() const { return bFirstInfo; }

    //inline bool isValid( int &x, int &y );
    inline void setFirstPixel(int &x, int &y, VIDEODATATYPE &d);
    void ppiDraw();
    //void extractVideoData( int i );

private:
    int xorg, yorg;
    int gain;
    int base;
    int range;//1:75km 2:150km 3:300km 4:500km
    int rangeweight;
    //bool bdivert;//偏心
    bool bFirstInfo;

    //DataPool *m_dp;
    int ** addr_near_x;
    int ** addr_near_y;
    int ** addr_far_x;
    int ** addr_far_y;
    FB_COLORTYPE *colortable;
    FB_COLORTYPE ** fbp;//pointer to framebuffer mem
    s_layer_unit ** layer;//frame buffer layer

    VIDEODATATYPE video_data[SAMPLE_NUM];//合并后的视频数据

    void ppiDraw( int azicnt );
};

#endif
