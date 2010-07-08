#ifndef _PPISEC_H_
#define _PPISEC_H_

#include <QObject>
#include <QMouseEvent>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <QTimer>

#include <vector>

#include "ppipainter.h"
#include "def.h"

#include "sysvalue.h"
#include "track.h"
#include "sectorregion.h"

//! \typedef VecTrk 航迹集
typedef std::vector<Track> VecTrk;
//! \typedef VecTrkIte 航迹集迭代器
typedef std::vector<Track>::iterator VecTrkIte;

//! 不明
const FB_COLORTYPE FB_SECOND_AMBIGUITY = 0x08d4fd;
//! 敌方
const FB_COLORTYPE FB_SECOND_ENEMY     = 0xff0000;
//! 友方
const FB_COLORTYPE FB_SECOND_COMPANY   = 0xc866fd;
//! 我方
const FB_COLORTYPE FB_SECOND_MYSELF    = 0x00ff00;


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

    //*************
    void secTimeOut();
    void addTrack(unsigned char *track);
    void delTrack(unsigned int);
    void drawSingleTrack(VecTrkIte ite);
    void clearSingleTrack(VecTrkIte ite);
    void drawAllTrack();
    void clearAllTrack();
    void delAllTrack();
    //*************

    void clearSurface();

    void setRangeWeight(int cont );
    void setDistantNum(int cont );
    /*
    void setRadius(int cont);
    void setCenter(int cx,int cy);
    */

    bool mousePress(QMouseEvent* e);
    void mouseMove(int cx,int cy);

    /*
    void disableDivert();
    void enableDivert();
    void enableSecondInfo();
    void disableSecondInfo();
    void freezeSecondInfo(int flag);
    */
    void setSectorRegion(int flag);
    void setWarnSectorRegion(int flag);
    //void sendSectorRegion(sectorRegionInfo *);

    //void drawHead(int azi);
signals:
    //void setSectorRegionInfo(unsigned char *);
    void showWarnFlag(int flag);

private:
    QTimer* secTimer;
    PPIPainter * painter;

    //PPIDispInfo *dispInfo;
    int headAzi;

    //******************
    VecTrk vectck;
    VecTrkIte ite;
    unsigned char sec_track[8];
    //******************

    SectorRegion *pSectorRegion;
    WarnSectorRegion *pWarnSectorRegion;
};

#endif

