#ifndef DEF_H
#define DEF_H

/*!
*    \file def.h
*    \brief 系统相关常量定义
*
*/

typedef unsigned long FB_COLORTYPE;

// 监视器分辨率


// PPI 显示区域相关常量定义
const int FB_WIDTH     = 1020;
const int FB_HEIGHT    = 1020;
const int PPI_R        = 1000; // PPI 直径
const int FB_XSTART    = 2;
const int FB_YSTART    = 2;


// PPI 数据
const int BYTES_PER_PIXEL=4; // 色深
const int SAMPLE_NUM=1000;   //一个视频量程合并后的采样点数
const int AZI_NUM=4096;      //雷达旋转一周的角度计数

const int ADDR_NEAR_AZI_LEN=AZI_NUM;//PPI显示不偏心时的一周显示角度总数
const int ADDR_NEAR_SMP_LEN=SAMPLE_NUM/2;//PPI显示不偏心时的一根视频显示点数

const int ADDR_FAR_AZI_LEN=AZI_NUM*2;//PPI显示偏心时的一周显示角度总数
const int ADDR_FAR_SMP_LEN=SAMPLE_NUM/2;//PPI显示偏心时的一根视频显示点数

const int RND_LEN=1040*1040;//余辉随机数表的长度,等于作图区域的分辨率

#endif // DEF_H
