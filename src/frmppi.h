#ifndef FRMPPI_H
#define FRMPPI_H

#include <QFrame>

class QLabel;
class QTimer;
class QPoint;

class FrmPPI: public QFrame
{
    Q_OBJECT

    public:

        /*!
        *    \brief PPI 量程类型
        *
        *    range type 定义了 PPI 具有的量程类型
        *
        *    - R_75\n
        *        75 km
        *    - R_150\n
        *        150 km
        *    - R_300\n
        *        300 km
        *    - R_500\n
        *        500 km
        *
        *    \sa setRange(), range()
        */
        /*
        enum RANGE
        {
            R_75  = 75,
            R_150 = 150,
            R_300 = 300,
            R_500 = 500
        };
        */

        FrmPPI(QWidget *parent = 0);

        /*!
        *    \brief 获取当前 PPI 原点坐标
        *    \return QPoint
        *    \see setOrigin(), resetOrigin()
        */
        //QPoint origin() const { return org; };

        /*!
        *    \brief 得到当前 PPI 量程
        *    \sa setRange()
        */
        //int range() const { return _range; };

    protected:
        void mouseMoveEvent(QMouseEvent *);
        void mousePressEvent(QMouseEvent *);

    public slots:
        void updateTime();
        //void setOrigin(QPoint &);
        //void resetOrigin();
        //void setRange(RANGE rangeType);
        void setWarnFlag(bool flag);

    signals:
        /*!
        *    \brief 发出警告信号
        *
        *    \param b true (有警告) or false (无警告)
        */
        void setWarnMask(bool b);

        //! 发出鼠标点击信号
        void mousePress(QMouseEvent *e);
        //! 发出鼠标移动信号
        void mouseMove(QMouseEvent *e);

    private:
        QLabel *labTitle;
        QLabel *labTime;
        QLabel *labPos;
        QLabel *warnTitle;
        QTimer *timer;

        /*
        QPoint org;

        RANGE _range;
        int rangeCal;
        */
        bool warnFlag;
        int warnCnt;
};

#endif // FRMPPI_H
