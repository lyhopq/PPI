#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>


#include "frmppi.h"
#include "ppipainter.h"
#include "ppisec.h"

namespace Ui {
    class MainWidget;
}

/*!
*    \class MainWidget
*    \brief 程序主界面
*/
class MainWidget : public QWidget {
    Q_OBJECT
public:
    MainWidget(QWidget *parent = 0);
    ~MainWidget();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::MainWidget *ui;

    FrmPPI *frameppi;

    DataPool *dp;
    PPIPainter *painter;
    PPISec     *ppisec;
};

#endif // MAINWIDGET_H
