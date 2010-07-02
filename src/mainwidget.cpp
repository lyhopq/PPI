#include <QHBoxLayout>

#include "mainwidget.h"
#include "ui_mainwidget.h"

#include "sysvalue.h"

#include <QDebug>

// 全局变量
sysValue *sysval;


MainWidget::MainWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWidget)
{
    ui->setupUi(this);

    frameppi = new FrmPPI(this);

    QHBoxLayout *layout = (QHBoxLayout *)this->layout();
    layout->addWidget(frameppi);


    sysval  = new sysValue;
    dp      = new DataPool;
    fblayer = new FBLayer(dp);
    painter = new PPIPainter(dp);
    ppisec  = new PPISec(painter);
}

MainWidget::~MainWidget()
{
    delete sysval;
    delete dp;
    delete fblayer;
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
