#ifndef DEF_H
#define DEF_H

/*!
*    \file def.h
*    \brief 系统相关常量定义
*
*/

//typedef unsigned short u16;
typedef int u16;

// 颜色信息
typedef unsigned long FB_COLORTYPE;
const FB_COLORTYPE FB_RED =   0xff0000;
const FB_COLORTYPE FB_GREEN = 0x00ff00;
const FB_COLORTYPE FB_BLUE =  0x0000ff;
const FB_COLORTYPE FB_YELLOW =0xffff00;
const FB_COLORTYPE FB_WHITE = 0xffffff;
const FB_COLORTYPE FB_BLACK = 0x000000;
const FB_COLORTYPE FB_CLEAR=0x000000;


// 监视器分辨率


// PPI 显示区域相关常量定义
const int FB_WIDTH     = 1020;
const int FB_HEIGHT    = 1020;
const int PPI_R        = 1000; // PPI 直径
const int PPI_r        = PPI_R/2; // PPI 半径


// PPI 数据
const int BYTES_PER_PIXEL=4; // 色深
const int SAMPLE_NUM=1000;   //一个视频量程合并后的采样点数
const int AZI_NUM=4096;      //雷达旋转一周的角度计数

const int ADDR_NEAR_AZI_LEN=AZI_NUM;//PPI显示不偏心时的一周显示角度总数
const int ADDR_NEAR_SMP_LEN=SAMPLE_NUM/2;//PPI显示不偏心时的一根视频显示点数

const int ADDR_FAR_AZI_LEN=AZI_NUM*2;//PPI显示偏心时的一周显示角度总数
const int ADDR_FAR_SMP_LEN=SAMPLE_NUM/2;//PPI显示偏心时的一根视频显示点数

const int RND_LEN=1000*1000;//余辉随机数表的长度,等于作图区域的分辨率

// 视频数据
/*!
*    \typedef VIDEODATATYPE
*
*    一次视频数据类型
*/
typedef unsigned short VIDEODATATYPE;
const int VIDEODATADEPTH = 10;
const int VIDEODATAMASK = 0x3ff;    // 10bits
const double VIDEODATALEN = 1024.0; // 2^10

/*!
*    \typedef VIDEODISPTYPE
*
*    一次信息显示类型
*/
typedef unsigned char VIDEODISPTYPE;//视频数据作图位宽
const int VIDEODISPDEPTH = 8;//视频点数据作图位宽
const int VIDEODISPMASK = 0xff;
const double VIDEODISPLEN = 256.0; // 2^8

#define FB_COLORTABLE_FILE     "./data/colortable32"

const double PI     = 3.14159;
const double HD     = 180.0/PI;



#endif // DEF_H
