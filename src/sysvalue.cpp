#include <string.h>
#include <stdio.h>

#include "sysvalue.h"
#include "def.h"

#include <QDebug>

FILE *timefp = NULL;

/*!
*    \brief 构造函数
*
*    如果 ./data/sysvalue.dat 文件存在，则从该文件中读取系统信息，否则使用默认系统参数。
*/
sysValue::sysValue()
{
    memset((void *)&sysVar,0,sizeof(struct systemVarDef));

    fp = NULL;
    if((fp = fopen("./data/sysvalue.dat","r+")) != NULL) // 文件存在
    {
        fread((void *)&sysVar,sizeof(struct systemVarDef),1,fp);
    }
    else    //只有在文件不存在时,设置初始化值
    {
        printf("\sysvalue.dat not exist!\n");


        fp = fopen("./data/sysvalue.dat","w");

        //设置默认参数
        sysVar.fs_time = 0;
        for(int i=0;i<5;i++)
        {
            sysVar.dispInfo.video_alalog_offset[i] = 0;//视频偏置
        }

        sysVar.shiPinWaChu_flag = 0;
        sysVar.shiPinWaChu_num = 0;
        sysVar.f_pljb = 0;
        sysVar.code_pl = 8;         //默认F9
        sysVar.f_bxjb = 0;
        sysVar.code_bx = 2;         //默认为非线性调频1
        sysVar.f_zqjb = 0;
        sysVar.code_zq = 4;         //默认为5ms
        sysVar.code_txzs = 0;
        sysVar.code_fszs = 0;       //发射关
        sysVar.code_lbq = 0xf001;   //
        sysVar.code_dianji = 0x01;  //默认为相参积累模式
        sysVar.code_shipin = 0x02;  //默认为背景视频模式
        sysVar.code_dismode = 0x03; //默认为综合显示
        sysVar.f_shansao = 0;       //默认圆扫

        sysVar.dispInfo.centerX = FB_WIDTH/2;   //中心点坐标
        sysVar.dispInfo.centerY = FB_HEIGHT/2;
        sysVar.dispInfo.rangePPI = 500;         //量程500公里
        sysVar.dispInfo.radiusPPI = PPI_R/2;    //半径
        sysVar.dispInfo.bFirstInfo = true;      //一次信息标志
        sysVar.dispInfo.bSecondInfo = true;     //二次信息标志
        sysVar.dispInfo.enadivert = 0;          //偏心显示使能标志
        sysVar.dispInfo.flagdivert = 0;         //偏心显示状态标志
        sysVar.dispInfo.bFreeze  = false;       //画面冻结标志
        sysVar.dispInfo.flagVideo = 1;          //背景视频
        sysVar.dispInfo.video_alalog_gain = 22; //模拟增益
        sysVar.dispInfo.video_digital_gain = 1; //数字增益
        sysVar.dispInfo.bit_cjb = 1;            //采集板自检

        sysVar.netcmd_ldkz = 1; //开机默认为允许发射
        sysVar.code_ldzt = 1;   //雷达自检总状态,1为正常

    }

    //*****
    resetCenter();
    setRange(500);
    sysVar.dispInfo.radiusPPI = PPI_R/2;    //半径
    sysVar.dispInfo.bFirstInfo = true;      //一次信息标志
    sysVar.dispInfo.bSecondInfo = true;     //二次信息标志
    sysVar.dispInfo.enadivert = 0;          //偏心显示使能标志
    sysVar.dispInfo.flagdivert = 0;         //偏心显示状态标志
    //*****

}

sysValue::~sysValue()
{
    if(fp != NULL)
    {
        saveSystemValue();
        fclose(fp);
    }
}

/*
//获取累积发射时间值
unsigned long sysValue::getFSTime()
{
    return(sysVar.fs_time);
}
*/

/*!
*    \brief 保存系统参数
*/
void sysValue::saveSystemValue()
{
    if(fp != NULL)
    {
        rewind(fp);
        fwrite((void *)&sysVar,sizeof(struct systemVarDef),1,fp);
    }
}

/*
//保存发射时间值
void sysValue::setFSTime(unsigned long fstime)
{
    sysVar.fs_time = fstime;
    saveSystemValue();
}

//读取系统的时间(毫秒ms)
unsigned long sysValue::getSystemTime()
{

}

//设置系统的BCD码时间
void sysValue::setSystemBCDTime()
{

}
*/

//! 打印系统信息
void sysValue::display()
{
    printf("\n##########################################\n");
    printf("CenterX: %d\t CenterY: %d\n", sysVar.dispInfo.centerX, sysVar.dispInfo.centerY);
    printf("Radious: %d\t Range: %d\n", sysVar.dispInfo.radiusPPI, sysVar.dispInfo.rangePPI);
    printf("isFirstInfo: %d\t isSecondInfo: %d\n", sysVar.dispInfo.bFirstInfo, sysVar.dispInfo.bSecondInfo);
    printf("isDivert: %d\t flagDivert: %d\n", sysVar.dispInfo.enadivert, sysVar.dispInfo.flagdivert);
    printf("isFreeze: %d\n", sysVar.dispInfo.bFreeze);
    printf("\n##########################################\n");
}
