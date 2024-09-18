#ifndef POINT2D_H
#define POINT2D_H

#include <cmath>

class Point2d
{
private:

    double m_x{};
    double m_y{};

public:

    Point2d(double x, double y): m_x{x}, m_y{y}
    {
    }

    Point2d() = delete; // no default construction

    bool operator<(const Point2d& pt) const { return sqrt(m_x*m_x + m_y*m_y) < sqrt(pt.m_x*pt.m_x + pt.m_y*pt.m_y); }

    Point2d& operator=(const Point2d& pt) = delete;

};

#endif