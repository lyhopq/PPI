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


// PPI 数据
const int BYTES_PER_PIXEL=4; // 色深
const int SAMPLE_NUM=1000;   //一个视频量程合并后的采样点数
const int AZI_NUM=4096;      //雷达旋转一周的角度计数

const int ADDR_NEAR_AZI_LEN=AZI_NUM;//PPI显示不偏心时的一周显示角度总数
const int ADDR_NEAR_SMP_LEN=SAMPLE_NUM/2;//PPI显示不偏心时的一根视频显示点数

const int ADDR_FAR_AZI_LEN=AZI_NUM*2;//PPI显示偏心时的一周显示角度总数
const int ADDR_FAR_SMP_LEN=SAMPLE_NUM/2;//PPI显示偏心时的一根视频显示点数

const int RND_LEN=1000*1000;//余辉随机数表的长度,等于作图区域的分辨率

// 视频数据 (10 bits)
typedef unsigned short VIDEODATATYPE;
const int VIDEODATADEPTH = 10;
const int VIDEODATAMASK = 0x3ff;    // 10bits
const double VIDEODATALEN = 1024.0;

#endif // DEF_H
