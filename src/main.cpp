#include <QtGui/QApplication>
#include <QFile>
#include <QTextCodec>
#include "mainwidget.h"

#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QTextCodec::setCodecForTr(QTextCodec::codecForLocale());

    QFile qss(":/res/ppi.css");
    if(qss.open(QIODevice::ReadOnly))
    {
        a.setStyleSheet(qss.readAll());
        qss.close();

        qDebug() << "set styleSheet";
    }

    MainWidget w;
    w.showFullScreen();
    return a.exec();
}
