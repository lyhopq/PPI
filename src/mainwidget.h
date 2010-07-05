#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>


namespace Ui {
    class MainWidget;
}

class QTimer;

class FrmPPI;
class DataPool;
class FBLayer;
class PPI;
class PPIPainter;
class PPISec;
class PPIThread;

/*!
*    \class MainWidget
*    \brief 程序主界面
*/
class MainWidget : public QWidget {
    Q_OBJECT
public:
    MainWidget(QWidget *parent = 0);
    ~MainWidget();

    void ppiUpdate();


    DataPool   *dp;

protected:
    void changeEvent(QEvent *e);

protected slots:
    void ppiUpdateSec();
    void toExit();

private:
    Ui::MainWidget *ui;

    FrmPPI *frameppi;

    FBLayer    *fblayer;
    PPI        *ppi;
    PPIPainter *painter;
    PPISec     *ppisec;
    PPIThread  *ppith;

    QTimer     *secTimer;
};

#endif // MAINWIDGET_H
