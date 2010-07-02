#ifndef _PPISEC_H_
#define _PPISEC_H_

#include <QObject>
#include <QMouseEvent>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <QTimer>

#include "ppipainter.h"
#include "def.h"

#include "sysvalue.h"


/*!
*    \class PPISec
*    \brief 绘制二次信息
*
*    调用 PPIPainter 画 PPI 刻度、距离圈等等。并且绘制二次信息。
*/
class PPISec : public QObject
{
    Q_OBJECT
public:
    PPISec( PPIPainter *p );
    ~PPISec();

public slots:
    void draw();
    //void secTimeOut();

    /*
    void setRangeWeight(int cont );//设置量程范围
    void setDistantNum(int cont );//设置距离圈
    void setRadius(int cont);//
    void setCenter(int cx,int cy);

    int mousePress(QMouseEvent* e);//设置圆心
    void mouseMove(int cx,int cy);
    */

    /*
    void disableDivert();
    void enableDivert();
    void enableSecondInfo();
    void disableSecondInfo();
    void freezeSecondInfo(int flag);
    void setSectorRegion(int flag);
    void setWarnSectorRegion(int flag);
    void sendSectorRegion(sectorRegionInfo *);
    */

    //void drawHead(int azi);
signals:
    //void setSectorRegionInfo(unsigned char *);
    //void showWarnFlag(int flag);
public:
    QTimer* secTimer;
    PPIPainter * painter;

    PPIDispInfo *dispInfo;
    int headAzi;
};

#endif

