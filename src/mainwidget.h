#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>


#include "frmppi.h"

namespace Ui {
    class MainWidget;
}

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
};

#endif // MAINWIDGET_H
