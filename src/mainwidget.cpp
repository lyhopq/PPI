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

    frameppi = new FrmPPI(this);

    QHBoxLayout *layout = (QHBoxLayout *)this->layout();
    layout->addWidget(frameppi);

    // 系统信息
    sysval  = new sysValue;

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
