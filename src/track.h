#ifndef _TRACK_H_
#define _TRACK_H_

#include <deque>    // STL

//! 航迹的最大点迹数
const int MAXDOTSIZE = 60;

/*!
*    \typedef DeqInt
*
*    \brief int 型双口队列
*/
typedef std::deque<int> DeqInt;

/*!
*    \typedef DeqIntIte
*
*    \brief \a DeqInt 迭代器
*
*    \see DeqInt
*/
typedef std::deque<int>::iterator DeqIntIte;

/*!
*    \class Track
*
*    \brief 航迹
*
*    提供一条航迹的信息及航迹的点集
*/
class Track
{
public:
    Track( int index );

    void append( int x,int y,int a );

    //! 获得航迹x坐标的起点
    DeqIntIte xbegin() { return _x.begin(); }
    //! 获得航迹x坐标的终点
    DeqIntIte xend() { return _x.end(); }
    //! 获得航迹y坐标的起点
    DeqIntIte ybegin() { return _y.begin(); }
    //! 获得航迹y坐标的终点
    DeqIntIte yend() { return _y.end(); }
    //! 获得航迹航向的起点
    DeqIntIte abegin() { return _a.begin(); }
    //! 获得航迹航向的终点
    DeqIntIte aend() { return _a.end(); }

    //! 获取航迹点迹个数
    unsigned int size() const { return m_size; }
    //! 获取航迹批号
    unsigned int index() const { return m_index; }

    //! 设置航迹属性
    void setAttr(unsigned int attr) { m_attr = attr; }
    //! 获取航迹属性
    unsigned int getAttr() const { return m_attr; }

private:

    DeqInt _x;
    DeqInt _y;
    DeqInt _a;//航向

    unsigned int m_index; //批号
    unsigned int m_attr;//属性
    unsigned int m_size;
};

#endif
