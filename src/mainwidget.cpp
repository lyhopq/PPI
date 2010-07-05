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


    sysval  = new sysValue;
    dp      = new DataPool;
    fblayer = new FBLayer(dp);
    ppi     = new PPI(dp);
    painter = new PPIPainter(dp);
    ppisec  = new PPISec(painter);

    ppith   = new PPIThread(this);
    ppith->start();

    secTimer = new QTimer(this);
    connect(secTimer, SIGNAL(timeout()), this, SLOT(ppiUpdateSec()));
    secTimer->setSingleShot(false);
    secTimer->start(1000); // 1s
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
