#ifndef DATEPOOL_H
#define DATEPOOL_H

#include <unistd.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/ioctl.h>
#include <sys/mman.h>

#include "def.h"

/*!
*    \class DataPool
*    \brief 加载数据并设置 FrameBuffer
*
*    从文件中读取数据表入内存，以便进行查表，提高数据吃力速度。
*    并打开 FrameBuffer 设备进行初始化。
*/

class DataPool
{
public:
    DataPool();
    ~DataPool();

    //从文件中读取的作图地址表
    int ** addr_near_x;
    int ** addr_near_y;
    int ** addr_far_x;
    int ** addr_far_y;

    //余辉表
    //int * randomx;
    //int * randomy;

    //一次色彩表
    //MFB_COLORTYPE *colortable;

    FB_COLORTYPE **fbp; //pointer to framebuffer mem

    //frame buffer layer
    //s_layer_unit ** layer;

    //pci share mem
    unsigned char * databuf;

    //s_ppi_azi *aziBuf;//方位数据缓存
    //s_ppi_video *videoBuf;//视频数据缓存
    //VecVwa vecvwa;//视频数据和方位数据的关联结果

    //用于显示的数据区:缓存一个视频数据
    //int dispnum;
    //s_ppi_video_for_disp *dispbuf;


    /*!
    *    \brief 提供视频数据
    *
    *    \param d 数据缓冲区指针
    */
    void setDataBuf(unsigned char * d){ databuf = d ;}

private:

    //frame buffer
    int fp;  // framebuffer file handler
    char *fbplinear;
    int bytes_per_pixel;
    unsigned long fbmemsize;

    int initDataTable();
    int initFbDev();
    int initFbLayer();

    void freeDataTable();
    void freeFbDev();
    void freeFbLayer();
};

#endif // DATEPOOL_H
