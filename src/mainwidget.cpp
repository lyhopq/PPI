#include <QHBoxLayout>

#include "mainwidget.h"
#include "ui_mainwidget.h"

#include "sysvalue.h"

#include "frmppi.h"
#include "datapool.h"
#include "fblayer.h"
#include "ppi.h"
#include "ppipainter.h"
#include "ppisec.h"
#include "ppithread.h"
#include "yh.h"

#include <QDebug>

// 全局变量
sysValue *sysval;
FILE     *fps; // 模拟一次数据文件


MainWidget::MainWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWidget)
{
    ui->setupUi(this);

    connect(ui->pbExit, SIGNAL(clicked()), this, SLOT(toExit()));

    //******************
    fps = fopen("data/data01.dat", "r");
    if(fps == NULL)
    {
        printf("\nthe data file data01.dat is not open!\n");
    }
    //******************

    bPreDivert = false;
    //bPreDivert = true;

    // 系统信息
    sysval  = new sysValue;

    frameppi = new FrmPPI(this);

    QHBoxLayout *layout = (QHBoxLayout *)this->layout();
    layout->addWidget(frameppi);

    // 数据
    dp      = new DataPool;

    // FrameBuffer 层
    fblayer = new FBLayer(dp);
    fblayer->setMaskRect(0, 0, 180, 50, true);
    fblayer->setMaskRect(FB_WIDTH - 200, 0, 200, 50, true);
    fblayer->setMaskRect(FB_WIDTH - 240, FB_HEIGHT - 50, 240, 50, true);
    fblayer->setMaskRect(850, 920, 70, 30, true);
    fblayer->setOffset(2, 2);

    // 一次视频
    ppi     = new PPI(dp);

    // PPI 绘制工具
    painter = new PPIPainter(dp);

    // 二次信息
    ppisec  = new PPISec(painter);

    // PPI 一次视频显示线程
    ppith   = new PPIThread(this);
    ppith->start();

    // 余辉
    yh = new Yh(dp);

    //********************************
    // 二次信息更新
    secTimer = new QTimer(this);
    connect(secTimer, SIGNAL(timeout()), this, SLOT(ppiUpdateSec()));
    secTimer->setSingleShot(false);
    secTimer->start(100); // 1s
    //********************************


    connect(frameppi, SIGNAL(mousePress(QMouseEvent*)), this, SLOT(mousePress(QMouseEvent*)));
    //connect(frameppi, SIGNAL(mouseMove(QMouseEvent*)), this, SLOT(mouseMove(QMouseEvent*)));

}

MainWidget::~MainWidget()
{
    //**********************
    if(fps != NULL)
    {
        fclose(fps);
    }
    //**********************
    delete sysval;
    delete dp;
    delete fblayer;
    delete ppi;
    delete painter;
    delete ppisec;

    delete ui;
}

void MainWidget::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

/*!
*    \brief 更新 PPI 一次显示
*
*    \note 该函数被线程调用
*/
void MainWidget::ppiUpdate()
{
    ppi->ppiDraw();
}

/*!
*    \brief 更新 PPI 二次显示
*/
void MainWidget::ppiUpdateSec()
{
    ppisec->draw();
}


void MainWidget::toExit()
{
    ppith->quit();

    close();
}

void MainWidget::mousePress(QMouseEvent* e)
{
    int x = e->x();
    int y = e->y();
    if(x > FB_WIDTH || y > FB_HEIGHT )
        return;
    if(ppisec->mousePress(e))//
    {
        //偏心显示,只通过鼠标左键确认
        //qDebug() << "%%%%%%%%%%%%%%%%%%%%" << e->button() << bPreDivert << "%%%%%%%%%%%%%%%%%";
        //*************
        //sysval->display();
        //*************
        if(e->button() == Qt::LeftButton && bPreDivert)
        {
            qDebug() << "divert!!!!!";
            //ppi->disableFirstInfo();
            sysval->setEnFirst(false);
            fblayer->clearFirst();
            fblayer->flushWithSecond();
            //ppi->enableDivert();
            sysval->setEnadivert(true);
            //ppi->setOrigin( x, y );
            sysval->setCenter(x, y);
            //frameppi->enableDivert();
            //frameppi->setOrigin( x, y );
            //ppi->enableFirstInfo();
            sysval->setEnFirst(true);
            bPreDivert = false;
        }

        //************
        //sysval->display();
        //************
    }
    /*
    //半自动录取
    else if(  iAutoTrack==2 && (e->button() == Qt::LeftButton || e->button() == Qt::RightButton))
    {
        x = x - dispInfo->centerX;
        y = dispInfo->centerY - y;
        double r = sqrt(x*x + y*y);     // P显半径
        if(r >= dispInfo->radiusPPI)    //超出范围
            return;
        r = r * ((double)dispInfo->rangePPI*2.0 / (double)dispInfo->radiusPPI); // 实际半径

        double a = atan2 ((double)x,(double)y)+2*PI*(x<0);  // 方位角(弧度)
        a = (a/PI)*180.0;   // 转换为度
        a = a/360.0*4096.0; // 所在扇区(4096个扇区)

        if(e->button() == Qt::LeftButton)   //鼠标左键不输入批号,右键输入批号
            emit(setAutoTrack(1,(int)a,(int)r));
        else
            emit(setAutoTrack(2,(int)a,(int)r));
    }
    //手动录取鼠标左键有效
    else if(  iAutoTrack==3 && (e->button() == Qt::LeftButton))
    {
        x = x - dispInfo->centerX;
        y = dispInfo->centerY - y;
        double r = sqrt(x*x + y*y);
        if(r >= dispInfo->radiusPPI)    //超出范围
            return;
        r = r * ((double)dispInfo->rangePPI*2.0 / (double)dispInfo->radiusPPI);

        double a = atan2 ((double)x,(double)y)+2*PI*(x<0);
        a = (a/PI)*180.0;
        a = a/360.0*4096.0;

        emit(setAutoTrack(3,(int)a,(int)r));
    }
    */
}

void MainWidget::mouseMove( QMouseEvent * e)
{
    int x = e->x();
    int y = e->y();
    if(x < FB_WIDTH && y < FB_HEIGHT )
    {
        ppisec->mouseMove( x, y );
    }
}
