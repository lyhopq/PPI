#include <stdio.h>
#include "ppi.h"


/*!
*    \brief 构造函数
*
*    \param dp    \a DataPool 指针
*    加载数据指针，并初始化一次显示参数。
*/
PPI::PPI( DataPool *dp )
{
    addr_near_x = dp->addr_near_x;
    addr_near_y = dp->addr_near_y;
    addr_far_x = dp->addr_far_x;
    addr_far_y = dp->addr_far_y;
    colortable = dp->colortable;
    fbp = dp->fbp;
    layer = dp->layer;

    //bdivert = 0;//偏心标志

    xorg = FB_WIDTH/2;//原点x坐标
    yorg = FB_HEIGHT/2;//原点y坐标

    gain = 1;
    base = 0;//有什么用？？？？？？？？？？？？
    range = 4;//500km
    rangeweight = 20;//500km 20 to 1
    bFirstInfo = 1;
}

//! 析构函数
PPI::~PPI()
{
}

/*!
*    \brief 设置原点
*
*    \param x    原点x坐标
*    \param y    原点y坐标
*
*    设置 PPI 显示的原点
*/
void PPI::setOrigin( int x, int y )
{
    if((x>=0) && (x<FB_WIDTH) && (y>=0) && (y<FB_HEIGHT))
    {
        xorg = x;
        yorg = y;
    }
}

//! 设置原点坐标为默认值
void PPI::resetOrigin()
{
    xorg = FB_WIDTH/2;
    yorg = FB_HEIGHT/2;
}

/*!
*    \brief 设置量程
*
*    \param r    量程类型
*
*    设置 PPI 显示的量程
*/
void PPI::setRange( int r )
{
    range = r;
    switch(range)
    {
        case 1 : //75km 3 to 1
            rangeweight = 3;
            break;
        case 2 : //150km 6 to 1
            rangeweight = 6;
            break;
        case 3 : //300km 12 to 1
            rangeweight = 12;
            break;
        case 4 : //500km 20 to 1
            rangeweight = 20;
            break;
        default :
            break;
    }
}


/*!
*    \brief 一次视频点
*
*    \param x    x 坐标
*    \param y    y 坐标
*    \param depth 数据点的值
*
*    设置屏幕的 (x,y) 处的一次视频数据为 depth
*
*    \see VIDEODATATYPE
*/
void PPI::setFirstPixel(int &x, int &y, VIDEODATATYPE &depth)
{
    VIDEODISPTYPE dp;

    //dp = (VIDEODISPTYPE)((depth*VIDEODISPLEN)/VIDEODATALEN);// 把10位彩色变为8为彩色
    dp = depth >> 2; // 把10位彩色变为8为彩色
    layer[y][x].nfirst = dp;
    if( !(layer[y][x].bsecond || layer[y][x].bmask ) )//没有二次视频数据或者屏蔽则
    {
        fbp[y][x] = colortable[dp];    //设置值
    }
}


extern FILE* fps; //fps--->data/data01_1.dat

/*!
*    \brief 绘制 PPI 一次视频
*
*    \note 现在只是使用模拟数据。数据文件为 data/data01.dat，
*          存储格式为：索引（4字节）＋方向值（2字节）＋数据（4096字节）
*    \warning data01.dat 文件按照 1024 制作，当前为 1000
*/
void PPI::ppiDraw()
{
    //VIDEODATATYPE Datafile[1024];
    static int k = 0;
    static int azi = 0;

    fseek(fps,k*4102+4,SEEK_SET);  // 跳过索引

    int angle_line;
    fread(&angle_line,2,1,fps);    //方位值

    //printf("angle_line=%d\n",angle_line);
    //fread(Datafile,2,1024,fps);//
    //memmove(video_data,Datafile,2048);
    fread(video_data, 2, SAMPLE_NUM, fps);    // 数据

    k++;
    if(k==15*4102) k=0;
    ppiDraw(azi);
    azi++;
    if(azi==4095) azi=0;


    /*
    //printf("m_dp->dispnum=%d\n",m_dp->dispnum);
    for(int i=0;i<m_dp->dispnum;i++)
    {
        extractVideoData(i);
        for(int j=0;j<m_dp->dispbuf[i].znum;j++)
        {
            ppiDraw( m_dp->dispbuf[i].azicnt[j] );
        }
    }
    */
}

/*
//将数据转换为长度为1024长度的数据 //数据合并
void PPI::extractVideoData( int i )//增益为倍数
{
    int vbiggest, vtmp;

    //提取视频数据,量程合并
    if(gain == 1)
    {
        for( int j=0; j<SAMPLE_NUM; j++ )
        {
            vbiggest = 0;
            for( int k=j*rangeweight;k<(j+1)*rangeweight;k++ )//k最大值为1024*20（SAMPLE_NUM*(rangeweihgt=5)(500km)）
            {
                if( k<CNTVIDEO )//20*1024
                {
                    vtmp = (m_dp->dispbuf[i].videodata[ k ])&VIDEODATAMASK;
                    if( vtmp > vbiggest )
                        vbiggest = vtmp;
                }
                else
                    vbiggest = 0;
            }//量程比大合并
            //增益
            vbiggest = vbiggest+base;
            vbiggest = (vbiggest>0)?vbiggest:0;
            vbiggest = (vbiggest>VIDEODATAMASK)?VIDEODATAMASK:vbiggest;
            video_data[j] = vbiggest;
        }
    }
    else if(gain == 2)
    {
        for( int j=0; j<SAMPLE_NUM; j++ )
        {
            vbiggest = 0;
            for( int k=j*rangeweight;k<(j+1)*rangeweight;k++ )
            {
                if( k<CNTVIDEO )
                {
                    vtmp = (m_dp->dispbuf[i].videodata[ k ])&VIDEODATAMASK;
                    if( vtmp > vbiggest )
                        vbiggest = vtmp;
                }
                else
                    vbiggest = 0;
            }//量程比大合并

            //增益
            vbiggest = (vbiggest+base)<<1;
            vbiggest = (vbiggest>0)?vbiggest:0;
            vbiggest = (vbiggest>VIDEODATAMASK)?VIDEODATAMASK:vbiggest;
            video_data[j] = vbiggest;
        }
    }
    else if(gain == 4)
    {
        for( int j=0; j<SAMPLE_NUM; j++ )
        {
            vbiggest = 0;
            for( int k=j*rangeweight;k<(j+1)*rangeweight;k++ )
            {
                if( k<CNTVIDEO )
                {
                    vtmp = (m_dp->dispbuf[i].videodata[ k ])&VIDEODATAMASK;
                    if( vtmp > vbiggest )
                        vbiggest = vtmp;
                }
                else
                    vbiggest = 0;
            }//量程比大合并

            //增益
            vbiggest = (vbiggest+base)<<2;
            vbiggest = (vbiggest>0)?vbiggest:0;
            vbiggest = (vbiggest>VIDEODATAMASK)?VIDEODATAMASK:vbiggest;
            video_data[j] = vbiggest;
        }
    }
    else if(gain == 8)
    {
        for( int j=0; j<SAMPLE_NUM; j++ )
        {
            vbiggest = 0;
            for( int k=j*rangeweight;k<(j+1)*rangeweight;k++ )
            {
                if( k<CNTVIDEO )
                {
                    vtmp = (m_dp->dispbuf[i].videodata[ k ])&VIDEODATAMASK;
                    if( vtmp > vbiggest )
                        vbiggest = vtmp;
                }
                else
                    vbiggest = 0;
            }//量程比大合并

            //增益
            vbiggest = (vbiggest+base)<<3;
            vbiggest = (vbiggest>0)?vbiggest:0;
            vbiggest = (vbiggest>VIDEODATAMASK)?VIDEODATAMASK:vbiggest;
            video_data[j] = vbiggest;
        }
    }
    else
    {
        for( int j=0; j<SAMPLE_NUM; j++ )
        {
            vbiggest = 0;
            for( int k=j*rangeweight;k<(j+1)*rangeweight;k++ )
            {
                if( k<CNTVIDEO )
                {
                    vtmp = (m_dp->dispbuf[i].videodata[ k ])&VIDEODATAMASK;
                    if( vtmp > vbiggest )
                        vbiggest = vtmp;
                }
                else
                    vbiggest = 0;
            }//量程比大合并

            //增益
            vbiggest = vbiggest+base;
            vbiggest = (vbiggest>0)?vbiggest:0;
            vbiggest = vbiggest*gain;
            vbiggest = (vbiggest>VIDEODATAMASK)?VIDEODATAMASK:vbiggest;
            video_data[j] = vbiggest;
        }
    }
}
*/

/*!
*    \brief 画某一方位的扫描线
*
*    \param azicnt    为方位角度
*/
void PPI::ppiDraw( int azicnt )
{
    int x, y;
    int readytobreak;
    VIDEODATATYPE v;

   if( isFirstInfo() )//是否显示一次
    {
        /*
        if( isDivert() )//是否偏心
        {
            readytobreak = 0;
            for( int i=0;i<ADDR_NEAR_SMP_LEN;i++ )
            {
                x = xorg+addr_near_x[azicnt][i];
                y = yorg-addr_near_y[azicnt][i];

                if( isValid(x,y) )//什么时候等于零？？？？？？？？
                {
                    setFirstPixel( x, y, video_data[i] );//画点…………video_data[i]是什么？？？？？？？？？？？？？视频数据缓冲
                }
                else
                {
                    //填充死区的地址表不是完全顺序的,一个越界地址的后面可能还有另一个不越界的地址…………？？？？？？？？？？？？？？？
                    if(readytobreak==5)
                    {
                        break;
                    }
                    else
                        readytobreak++;
                }
            }

            readytobreak = 0;
            for( int i=0;i<ADDR_FAR_SMP_LEN;i++ )
            {
                x = xorg+addr_far_x[2*(azicnt)][i];
                y = yorg-addr_far_y[2*(azicnt)][i];

                if( isValid(x,y) )
                {
                    setFirstPixel( x, y, video_data[i+ADDR_NEAR_SMP_LEN] );
                }
                else
                {
                    if(readytobreak==10)
                    {
                        break;
                    }
                    else
                        readytobreak++;
                }

                x = xorg+addr_far_x[2*(azicnt)+1][i];
                y = yorg-addr_far_y[2*(azicnt)+1][i];

                if( isValid(x,y) )
                {
                    setFirstPixel( x, y, video_data[i+ADDR_NEAR_SMP_LEN] );
                }
                else
                {
                    if(readytobreak==10)
                    {
                        break;
                    }
                    else
                        readytobreak++;
                }
            }
        }
        */
        //else    // 不偏心只显示近区??????
        {
            //readytobreak = 0;
            for( int i=0;i<ADDR_NEAR_SMP_LEN;i++ )
            {
                x = xorg+addr_near_x[azicnt][i];
                y = yorg-addr_near_y[azicnt][i];

                //if( isValid(x,y) )//判断坐标是否有效
                {
                    v = ( video_data[i*2]>video_data[i*2+1] )?video_data[i*2]:video_data[i*2+1];//二合一比大
                    setFirstPixel( x, y, v );
                }
                /*
                else
                {
                    if(readytobreak==5)
                    {
                        break;
                    }
                    else
                        readytobreak++;
                }
                */
            }
        }
    }

}
