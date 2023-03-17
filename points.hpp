#ifndef POINTS_HPP
#define POINTS_HPP
#include <iostream>
#include <cmath>
#include <array>

struct Point3D{
    double m_x=0, m_y=0, m_z=0;

    constexpr Point3D() = default;
    constexpr Point3D(double x, double y, double z) : m_x(x), m_y(y), m_z(z){};

    double norm () const;

    void normalize();

    Point3D& Rot_x(double angle);
    Point3D& Rot_x(double const& s, double const& c);

    Point3D& Rot_y(double angle);
    Point3D& Rot_y(double const& s, double const& c);

    Point3D& Rot_z(double angle);
    Point3D& Rot_z(double const& s, double const& c);

    Point3D operator -(Point3D p2) const {return {m_x - p2.m_x, m_y - p2.m_y, m_z - p2.m_z};};

    bool operator ==(Point3D p2) const {return m_x == p2.m_x && m_y == p2.m_y && m_z == p2.m_z;}

    friend std::ostream& operator <<(std::ostream& of, Point3D const p) {return of << p.m_x << " " << p.m_y << " " << p.m_z;}
};

double scal(Point3D const& p1, Point3D const& p2);

#endif