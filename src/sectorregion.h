#ifndef SECTORREGION_H
#define SECTORREGION_H

#include <QList>
#include "def.h"

typedef struct sectorRegionInfo
{
      int id;
      int r1;
      int r2;
      int degree1;
      int degree2;
      int x1;
      int y1;
      int x2;
      int y2;
      int flag;
};


class PPIPainter;
class PPISec;

class SectorRegion
{
public:
  SectorRegion(PPIPainter *p, int maxrgnums=1, FB_COLORTYPE col=0);

  virtual ~SectorRegion();

public:
     PPISec *pOwner;
     //void setCenter(int cx,int cy);
     int  getSectorRegionId();
     void setMovePoint(int x,int y);
     void setPressPoint(int flag,int x,int y);
     void cancel();
     void changeRadius(int radius);
     void changeRange(int range);

     void clearAllSectorRegion();
     void clearSigleSectorRegion();
     void clearSigleSectorRegion(sectorRegionInfo *psrlist);
     void drawSigleSectorRegion();
     virtual void drawAllSectorRegion();
     virtual void setSectorRegion(int flag);
     void delSigleSectorRegion(int flag);

     //! 扇区是否使能
     bool isEnRegion() const { return regionEna; }
     //! 设置扇区使能
     void setENRegion(bool b) { regionEna = b; }

     void displayRegion(sectorRegionInfo *p);

     bool firstflag;
     bool regionEna;
     PPIPainter * painter;
     QList<struct sectorRegionInfo*> sectorlist;
     sectorRegionInfo *psrlist;
     int  maxSectorRegions;
     int regionNums;
     int regionFlag;//挖区标志
     //int radiusPPI;
     //int rangePPI;//量程
     //int centerX,centerY;
     FB_COLORTYPE color;
};

class WarnSectorRegion:public SectorRegion
{
public:
     WarnSectorRegion(PPIPainter *p, int maxrgnums=1, FB_COLORTYPE col=0);
     void warnJudge(int x,int y);

     void drawAllSectorRegion();
     void setSectorRegion(int flag);
     int getWarnFlag();
private:
     bool warnflag;
};

#endif
