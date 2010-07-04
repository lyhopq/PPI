#include "ppithread.h"
#include "mainwidget.h"

/*!
*    \brief 构造函数
*
*    \param parent    MainWidget 指针
*/
PPIThread::PPIThread(MainWidget *parent)
{
    mw = parent;

    runflag = true;
}

/*
PPIThread::~PPIThread()
{
    //quit();
}
*/

//! 调用 ppiUpdate()
void PPIThread::run()
{
    while(runflag)
    {
        //mw->ppiUpdate(mw->dp->databuf);
        mw->ppiUpdate();
    }
}

//! 线程退出
void PPIThread::quit()
{
    runflag = false;
    QThread::quit();
}
