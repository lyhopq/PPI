#include <QLabel>
#include <QTimer>
#include <QPalette>
#include <QRect>
#include <QDateTime>
#include <QMouseEvent>
#include <QBitmap>

#include <math.h>

#include "frmppi.h"
#include "sysvalue.h"
#include "def.h"

const int M_WIDTH = 24;
const int M_HEIGHT = 24;

static unsigned char cb_bits[] =  // cursor bitmap
{
   0x00, 0x00, 0x00,
   0x00, 0x00, 0x00,
   0x00, 0x00, 0x00,
   0x00, 0x00, 0x00,
   0x00, 0x00, 0x00,
   0x00, 0x00, 0x00,
   0x00, 0x00, 0x00,
   0x00, 0x00, 0x00,
   0x00, 0x00, 0x00,
   0x00, 0x00, 0x00,
   0x00, 0x00, 0x00,
   0x00, 0x00, 0x00,
   0x00, 0x00, 0x00,
   0x00, 0x00, 0x00,
   0x00, 0x00, 0x00,
   0x00, 0x00, 0x00,
   0x00, 0x00, 0x00,
   0x00, 0x00, 0x00,
   0x00, 0x00, 0x00,
   0x00, 0x00, 0x00,
   0x00, 0x00, 0x00,
   0x00, 0x00, 0x00,
   0x00, 0x00, 0x00,
   0x00, 0x00, 0x00,
   0x00, 0x00, 0x00,
   0x00, 0x00, 0x00,
   0x00, 0x00, 0x00,
   0x00, 0x00, 0x00,
   0x00, 0x00, 0x00,
   0x00, 0x00, 0x00,
   0x00, 0x00, 0x00,
   0x00, 0x00, 0x00};

static unsigned char cm_bits[] =  // cursor bitmap mask
{
   0x00, 0x18, 0x00,
   0x00, 0x18, 0x00,
   0x00, 0x18, 0x00,
   0x00, 0x18, 0x00,
   0x00, 0x18, 0x00,
   0x00, 0x18, 0x00,
   0x00, 0x18, 0x00,
   0x00, 0x18, 0x00,
   0x00, 0x18, 0x00,
   0x00, 0x18, 0x00,
   0x00, 0x18, 0x00,
   0xff, 0xff, 0xff,
   0xff, 0xff, 0xff,
   0x00, 0x18, 0x00,
   0x00, 0x18, 0x00,
   0x00, 0x18, 0x00,
   0x00, 0x18, 0x00,
   0x00, 0x18, 0x00,
   0x00, 0x18, 0x00,
   0x00, 0x18, 0x00,
   0x00, 0x18, 0x00,
   0x00, 0x18, 0x00,
   0x00, 0x18, 0x00,
   0x00, 0x18, 0x00
};

extern sysValue *sysval;

/*!
*    \class FrmPPI
*    \brief FrmPPI 为PPI显示提供一些显示信息及相关设置
*
*    FrmPPI 为PPI显示提供诸如：标签、当前位置、警告信息、系统时间等信息。还可以设置PPI显示的原点坐标，
*    和显示量程。
*/


/*!
*    构造函数
*
*
*/
FrmPPI::FrmPPI(QWidget *parent)
    : QFrame(parent)
{
    //_range = R_500;
    //rangeCal = 500.0;

    //resetOrigin(); // 设置 PPI 原点

    setFixedSize(FB_WIDTH, FB_HEIGHT);

    warnFlag = false;

    QPalette palette;
    palette.setColor(this->backgroundRole(), Qt::black);

    this->setAutoFillBackground(true);
    this->setPalette(palette);

    // 标题
    labTitle = new QLabel(this);
    labTitle->setGeometry(QRect(0,0,180,50));
    labTitle->setAlignment(Qt::AlignCenter);
    labTitle->setFrameShape(QFrame::StyledPanel);
    labTitle->setFrameShadow(QFrame::Raised);
    labTitle->setLineWidth(2);
    labTitle->setText(tr("XX 型雷达\n终端显控台"));

    // 时间
    labTime = new QLabel(this);
    labTime->setGeometry(QRect(FB_WIDTH - 200,0,200,50));
    labTime->setAlignment(Qt::AlignCenter);
    labTime->setFrameShape(QFrame::StyledPanel);
    labTime->setFrameShadow(QFrame::Raised);
    labTime->setLineWidth(2);

    // 坐标
    labPos = new QLabel(this);
    labPos->setGeometry(QRect(FB_WIDTH - 240,FB_HEIGHT - 50,240,50));
    labPos->setAlignment(Qt::AlignCenter);
    labPos->setFrameShape(QFrame::StyledPanel);
    labPos->setFrameShadow(QFrame::Raised);
    labPos->setLineWidth(2);
    QString tmp = tr("X：%1 Y：%2\n 距离：%3 方位：%4")
                .arg(0, 4, 10).arg(0, 4, 10)
                .arg(0, 6, 'f', 2).arg(0, 6, 'f', 2);
    labPos->setText(tmp);


    // 警告
    warnTitle = new QLabel(this);
    warnTitle->setObjectName("warnTile");
    warnTitle->setGeometry(QRect(850,920,70,30));
    warnTitle->setText(tr("告 警"));

    // 定时器
    timer = new QTimer(this);
    timer->setSingleShot(false);
    timer->start(500); // 0.5s
    connect(timer, SIGNAL(timeout()), this, SLOT(updateTime()));

    // 鼠标
    const QSize size(M_WIDTH, M_HEIGHT);
    const QBitmap cb = QBitmap::fromData(size, cb_bits);
    const QBitmap cm = QBitmap::fromData(size, cm_bits);
    const QCursor custom( cb, cm );   // create bitmap cursor

    setCursor(custom);
    this->setMouseTracking(true); // 开启鼠标跟踪
}

/*!
*    \brief 设置警告信息
*
*    \param flag true (警告) or false (不警告)
*/
void FrmPPI::setWarnFlag(bool flag)
{
    warnFlag = flag;
    warnCnt = 8;
    //emit setWarnMask(true);
}

/*!
*    \brief 更新时间信息
*
*    同步显示系统时间信息，如果设置了警告标志 \a warnFlag 则显示警告信息。
*/
void FrmPPI::updateTime()
{
    QDateTime datetime = QDateTime::currentDateTime();

    int y = datetime.date().year();
    int m = datetime.date().month();
    int d = datetime.date().day();
    int h = datetime.time().hour();
    int f = datetime.time().minute();
    int s = datetime.time().second();
    QString tmp = tr("%1 年 %2 月 %3 日\n %4 时 %5 分 %6 秒")
                    .arg(y).arg(m).arg(d)
                    .arg(h).arg(f).arg(s);

    labTime->setText(tmp);

    if(warnFlag) // 设置了警告标志
    {
        if(warnCnt != 0) // 实现警告闪烁
        {
            warnCnt--;
            if(warnCnt%2)
                warnTitle->setStyleSheet("color: black");
            else
                warnTitle->setStyleSheet("color: red");
        }
        else
        {
            warnTitle->setStyleSheet("color: black");
            warnFlag = false;
            //emit setWarnMask(false);
        }
    }
}

//! 处理鼠标移动事件。主要用于获取PPI的当前位置
void FrmPPI::mouseMoveEvent(QMouseEvent *e)
{
    int x, y, rp;
    double r, a;

    // 获取屏幕坐标
    x = e->x();
    y = e->y();

    // 得到 PPI 坐标
    x = x - sysval->getCenterX();
    y = sysval->getCenterY() - y;

    // 获取半径
    rp = sqrt(x*x + y*y);
    r = rp*((double)sysval->getRange()/PPI_r);

    // 获取方位角
    a = atan2((double)x, (double)y) + 2*PI*(x < 0 ? 1:0);
    a = a*HD;

    // 设置位置标签
    QString tmp = tr("X：%1 Y：%2\n 距离：%3 方位：%4")
                .arg(x, 4, 10).arg(y, 4, 10)
                .arg(r, 6, 'f', 2).arg(a, 6, 'f', 2);

    labPos->setText(tmp);

    QWidget::mouseMoveEvent(e);

    emit mouseMove(e);
}

//! 处理鼠标点击事件
void FrmPPI::mousePressEvent(QMouseEvent *e)
{
    QWidget::mousePressEvent(e);

    emit mousePress(e);
}

/*!
*    \brief 设置 PPI 原点
*
*    \param p 要设置的原点坐标
*
*    \sa origin(), resetOrigin()
*
*    \note 坐标为 Widget 的直角坐标值
*/
/*
void FrmPPI::setOrigin(QPoint &p)
{
    int x = p.x();
    int y = p.y();

    if((x >= 0) && (x < FB_WIDTH) && (y >= 0) && (y < FB_HEIGHT))
    {
        org.setX(x);
        org.setY(y);
    }
}
*/

/*
//! 设置 PPI 原点为默认
void FrmPPI::resetOrigin()
{
    org.setX(FB_WIDTH/2);
    org.setY(FB_HEIGHT/2);
}
*/

/*!
*    \brief 设置 PPI 量程
*
*    \param rangeType 量程类型
*
*    \sa range()
*/
/*
void FrmPPI::setRange(RANGE rangeType)
{
    _range = rangeType;
}
*/
