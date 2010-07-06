#include <QTimer>

#include <QDebug>
#include "yh.h"

/*!
*    \brief 构造函数
*
*    \param dp    DataPool 指针
*
*    加载数据指针，初始化余辉变量
*/
Yh::Yh( DataPool *dp )
{
    timer = new QTimer(this);
    timer->setSingleShot(false);
    timer->start(20);

    randomx = dp->randomx;
    randomy = dp->randomy;
    colortable = dp->colortable;
    fbp = dp->fbp;
    layer = dp->layer;

    bYh = 1;
    yh_index = 0;
    yhLen=YH_NUM;
    setYhWeight(6);

    connect(timer,SIGNAL(timeout()),this,SLOT(yh()));
}

/*!
*    \brief 设置余辉衰减级别
*
*    \param    level    余辉衰减级别
*
*    \note 级别有16级 (1~16)，级别越小衰减越快。 level = 16 无衰减。
*/
void Yh::setYhWeight(int level)
{
    switch(level)
    {
        case 1: yhLen=YH_NUM; yhDecWeight=160; break;
        case 2: yhLen=YH_NUM; yhDecWeight=150; break;
        case 3: yhLen=YH_NUM; yhDecWeight=140; break;
        case 4: yhLen=YH_NUM; yhDecWeight=130; break;
        case 5: yhLen=YH_NUM; yhDecWeight=120; break;
        case 6: yhLen=YH_NUM; yhDecWeight=110; break;
        case 7: yhLen=YH_NUM; yhDecWeight=100; break;
        case 8: yhLen=YH_NUM; yhDecWeight=90; break;
        case 9: yhLen=YH_NUM; yhDecWeight=80; break;
        case 10: yhLen=YH_NUM; yhDecWeight=70; break;
        case 11: yhLen=YH_NUM; yhDecWeight=60; break;
        case 12: yhLen=YH_NUM; yhDecWeight=50; break;
        case 13: yhLen=YH_NUM; yhDecWeight=40; break;
        case 14: yhLen=YH_NUM; yhDecWeight=30; break;
        case 15: yhLen=YH_NUM; yhDecWeight=20; break;
        case 16: yhLen=0; yhDecWeight=0; break;

        default:break;
    }
}

//! 余辉绘制函数
void Yh::yh()
{
    int x,y;

    if(bYh)
    {
        for(int i = 0; i < yhLen; i++)
        {
            x = randomx[(yh_index+i)%RND_LEN];
            y = randomy[(yh_index+i)%RND_LEN];

            if(x > PPI_R || y > PPI_R)
                continue;
            //qDebug() << x << y;
            //layer[y][x].nfirst = (layer[y][x].nfirst>yhDecWeight)?(layer[y][x].nfirst-yhDecWeight):0;

            layer[y][x].nfirst = ((layer[y][x].nfirst-yhDecWeight)>0)?(layer[y][x].nfirst-yhDecWeight):0;

            if(!(layer[y][x].bsecond || layer[y][x].bmask))
            {
                fbp[y][x] = colortable[layer[y][x].nfirst];
            }
        }

            yh_index = (yh_index+yhLen)%RND_LEN;
        }
}
