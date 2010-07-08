#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>


namespace Ui {
    class MainWidget;
}

class QTimer;
class QMouseEvent;

class FrmPPI;
class DataPool;
class FBLayer;
class PPI;
class PPIPainter;
class PPISec;
class PPIThread;
class Yh;

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

    void mousePress(QMouseEvent *);
    void mouseMove(QMouseEvent *);

private:
    Ui::MainWidget *ui;

    FrmPPI *frameppi;

    FBLayer    *fblayer;
    PPI        *ppi;
    PPIPainter *painter;
    PPISec     *ppisec;
    PPIThread  *ppith;
    Yh         *yh;

    QTimer     *secTimer;

    bool bPreDivert;
};

#endif // MAINWIDGET_H
