#ifndef _YH_H_
#define _YH_H_

#include <QObject>

#include "def.h"
#include "datapool.h"

const int YH_NUM=500*50;

class QTimer;

/*!
*    \class Yh
*    \brief 余辉
*
*    添加模拟余辉效果
*/
class Yh : public QObject
{
    Q_OBJECT

public:
    Yh( DataPool *dp );

public:
    void setYhWeight( int level);

    //! 余辉使能
    void enableYh(){ bYh = 1; }
    //! 余辉不使能
    void disableYh(){ bYh = 0; }
    //! 余辉是否使能
    bool isYh() const { return bYh; }

private slots:
    void yh();

private:
    QTimer *timer;

    int yhDecWeight;
    int yhLen;
    int yh_index;
    bool bYh;

    int * randomx;
    int * randomy;
    FB_COLORTYPE *colortable;
    FB_COLORTYPE ** fbp;//pointer to framebuffer mem
    s_layer_unit ** layer;//frame buffer layer
};

#endif
