#ifndef ARRAYS_HPP
#define ARRAYS_HPP
#include <iostream>
#include <math.h>

struct Point3D{
    double m_x=0, m_y=0, m_z=0;

    constexpr Point3D(){}
    constexpr Point3D(double x, double y, double z) : m_x(x), m_y(y), m_z(z){};

    double norm () const;

    void normalize();

    Point3D& Rot_x(double angle);

    Point3D& Rot_y(double angle);

    Point3D& Rot_z(double angle);

    Point3D operator -(Point3D p2) const {return {m_x - p2.m_x, m_y - p2.m_y, m_z - p2.m_z};};

    friend std::ostream& operator <<(std::ostream& of, Point3D const p) {of << p.m_x << " " << p.m_y << " " << p.m_z; return of;}
};

double scal(Point3D const& p1, Point3D const& p2);

Point3D sub(Point3D const& p1, Point3D const& p2);

#endif