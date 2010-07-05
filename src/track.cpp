#include "track.h"

/*!
*    \brief 构造函数
*
*    \param index    航迹的批号
*
*    生成一条航迹，其批号为 index。
*/
Track::Track( int index )
{
    m_index = index;
    m_size = 0;
}

/*!
*    \brief 加入新的点迹
*
*    \param x    点迹x坐标
*    \param y    点迹y坐标
*    \param a    点迹方位值
*
*    把一个坐标为 (x,y)，方位为 a 的点迹加入到航迹的点集。
*
*    \note 航迹的点集的固定大小为 \a MAXDOTSIZE。
*/
void Track::append( int x,int y,int a )
{
    if( m_size == MAXDOTSIZE ) // 如果点迹数大于 MAXDOTSIZE，则去掉最早的点迹
    {
        _x.pop_front();
        _y.pop_front();
        _a.pop_front();
    }
    else
    {
        m_size ++;
    }

    _x.push_back( x );
    _y.push_back( y );
    _a.push_back( a );
}
