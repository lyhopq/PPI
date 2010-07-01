#ifndef  SYSVALUE_H
#define  SYSVALUE_H

#include <stdio.h>

typedef struct PPIDispInfo
{
    int centerX;                    //坐标中心
    int centerY;
    int radiusPPI;                  //半径
    int rangePPI;                   //量程
    int enadivert;                  //
    int flagdivert;
    bool bFirstInfo;                // 一次显示
    bool bSecondInfo;               // 二次显示
    bool bFreeze;                   // 画面冻结
    int  flagVideo;                 // 视频通道选择
    int video_alalog_gain;          //模拟增益
    int video_digital_gain;         //数字增益
    int video_alalog_offset[5];     //视频1偏置
    unsigned char bit_cjb;          //采集板自检
};

//BCD码格式
typedef struct systemBCDTime
{
    unsigned char tv_sec;           //秒
    unsigned char tv_min;           //分
    unsigned char tv_hour;          //小时
    unsigned char tv_reserve;       //保留
    unsigned char tv_day;           //日
    unsigned char tv_month;         //月
    unsigned char tv_yearl;         //年(低)
    unsigned char tv_yearh;         //年(高)
};

typedef struct systemTime_struct
{
    unsigned long fs_time;
    unsigned long tv_ms;            //毫秒ms
    systemBCDTime tv_bcd;           //BCD码时间
};

typedef struct systemVarDef{
    unsigned long fs_time;          // 累积发射时间
    unsigned char shiPinWaChu_flag; //视频挖除标志
    unsigned char shiPinWaChu_num;  //挖区数
    unsigned char f_pljb;           // 频率捷变
    unsigned char code_pl;          // 频率
    unsigned char f_bxjb;           // 波形捷变
    unsigned char code_bx;          // 波形
    unsigned char f_zqjb;           // 周期捷变
    unsigned char code_zq;          // 周期
    unsigned char code_txzs;        // 天线转速
    unsigned char code_fszs;        // 发射
    unsigned int code_lbq;          // 滤波器
    unsigned char code_dianji;      // 点迹
    unsigned char code_shipin;      // 视频
    unsigned char code_dismode;     // 显示方式
    unsigned int  code_shansao;     // 扇扫
    unsigned char  f_shansao;
    PPIDispInfo dispInfo;           // p显信息
    //以下保留128个字节
    unsigned char netcmd_ldkz;
    unsigned char code_ldzt;
    unsigned char reserve[126];//128-34
};


/*!
*    \class sysValue
*    \brief 保存系统的相关信息
*
*/
class sysValue
{
public:
    sysValue();

    //unsigned long getFSTime();
    //void setFSTime(unsigned long fstime);

    /*!
    *    \brief 获得系统参数指针
    */
    systemVarDef *getSystemValue() { return &sysVar; };

    /*!
    *    \brief 获得 PPI 显示参数指针
    */
    PPIDispInfo *getDispInfo() { return &(sysVar.dispInfo); };
    void saveSystemValue();

    //unsigned long getSystemTime();
    //void setSystemBCDTime();

private:
    //systemTime_struct sysTime;
    FILE *fp;
    systemVarDef sysVar;

};

#endif   /* ----- #ifndef SYSVALUE_H  ----- */
