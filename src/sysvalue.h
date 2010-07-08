#ifndef  SYSVALUE_H
#define  SYSVALUE_H

#include <stdio.h>

#include "def.h"

/*!
*    \struct PPIDispInfo
*    \brief PPI 显示参数
*
*    集合了 PPI 的显示参数
*/
typedef struct PPIDispInfo
{
    int centerX;                    //坐标中心
    int centerY;
    int radiusPPI;                  //半径
    int rangePPI;                   //量程
    bool enadivert;                  //偏心使能
    bool flagdivert;                 //偏心标志
    bool bFirstInfo;                // 一次显示
    bool bSecondInfo;               // 二次显示
    bool bFreeze;                   // 画面冻结
    int  flagVideo;                 // 视频通道选择
    int video_alalog_gain;          //模拟增益
    int video_digital_gain;         //数字增益
    int video_alalog_offset[5];     //视频1偏置
    unsigned char bit_cjb;          //采集板自检
};

/*
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
*/

/*!
*    \struct systemVarDef
*    \brief 系统参数
*
*    集合了系统参数。包括 PPI 显示信息 \a dispInfo。
*
*    \sa PPIDispInfo
*/
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
    ~sysValue();

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

    //! 取得原点x坐标
    int getCenterX() const { return sysVar.dispInfo.centerX; }
    //! 取得原点y坐标
    int getCenterY() const { return sysVar.dispInfo.centerY; }
    //! 取得半径
    int getRadius() const { return sysVar.dispInfo.radiusPPI; }
    //! 取得量程
    int getRange() const { return sysVar.dispInfo.rangePPI; }
    //! 偏心是否使能
    bool isDivertEnable() const { return sysVar.dispInfo.enadivert; }
    //! 取得偏心状态
    bool isDivert() const { return sysVar.dispInfo.flagdivert; }
    //! 一次显示是否使能
    bool isFirstInfo() const { return sysVar.dispInfo.bFirstInfo; }
    //! 二次显示是否使能
    bool isSecondInfo() const { return sysVar.dispInfo.bSecondInfo; }
    //! 画面冻结是否使能
    bool isFreeze() const { return sysVar.dispInfo.bFreeze; }

    //! 设置默认原点
    void resetCenter()
    {
        sysVar.dispInfo.centerX = FB_WIDTH/2;
        sysVar.dispInfo.centerY = FB_HEIGHT/2;
    }
    //! 设置原点
    void setCenter(int x, int y)
    {
        if((x>=0) && (x<FB_WIDTH) && (y>=0) && (y<FB_HEIGHT))
        {
            sysVar.dispInfo.centerX = x;
            sysVar.dispInfo.centerY = y;
        }
    }

    //! 设置半径
    void setRadious(int r) { sysVar.dispInfo.radiusPPI = r; }
    //! 设置量程
    void setRange(int r) { sysVar.dispInfo.rangePPI = r; }
    //! 设置偏心使能
    void setEnadivert(bool b) { sysVar.dispInfo.enadivert = b; }
    //! 设置偏心状态
    void setDivert(bool b) { sysVar.dispInfo.flagdivert = b; }
    //! 设置一次显示使能
    void setEnFirst(bool b) { sysVar.dispInfo.bFirstInfo = b; }
    //! 设置二次显示使能
    void setEnSecond(bool b) { sysVar.dispInfo.bSecondInfo = b; }
    //! 设置画面冻结
    void setEnFreeze(bool b) { sysVar.dispInfo.bFreeze = b; }

    void display();

    //unsigned long getSystemTime();
    //void setSystemBCDTime();

private:
    //systemTime_struct sysTime;
    FILE *fp;
    systemVarDef sysVar;

};

#endif   /* ----- #ifndef SYSVALUE_H  ----- */
