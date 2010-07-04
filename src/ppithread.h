#ifndef __PPITHREAD_H__
#define __PPITHREAD_H__

#include <QThread>
#include <string.h>


class MainWidget;

/*!
*    \brief PPI 绘制线程
*
*    调用 MainWidget.ppiUpdate() 更新 PPI 一次数据
*/
class PPIThread: public QThread
{
    Q_OBJECT
public:

    PPIThread(MainWidget *parent);
    //~PPIThread();
    void quit();

protected:
    void run();

private:
    MainWidget *mw;

    bool runflag;
};

#endif
