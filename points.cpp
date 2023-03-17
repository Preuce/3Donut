#include "points.hpp"

/**
 * @brief Rotates a point by a given angle on the x axis
 * 
 * @param angle 
 * @return Point3D& 
 */
Point3D& Point3D::Rot_x(double angle){
    double new_y = m_y*cos(angle) + m_z*sin(angle);
    double new_z = -m_y*sin(angle) + m_z*cos(angle);

    m_y = new_y;
    m_z = new_z;

    return *this;
}

/**
 * @brief Rotates a point by a given angle on the y axis
 * 
 * @param angle 
 * @return Point3D& 
 */
Point3D& Point3D::Rot_y(double angle){
    double new_x = m_x*cos(angle) - m_z*sin(angle);
    double new_z = m_x*sin(angle) + m_z*cos(angle);

    m_x = new_x;
    m_z = new_z;

    return *this;
}

/**
 * @brief Rotates a point by a given angle on the z axis
 * 
 * @param angle 
 * @return Point3D& 
 */
Point3D& Point3D::Rot_z(double angle){
    double new_x = m_x*cos(angle) - m_y*sin(angle);
    double new_y = m_x*sin(angle) + m_y*cos(angle);

    m_x = new_x;
    m_y = new_y;

    return *this;
}

/** norm
 * @brief Computes the 2-norm of a vector
 * 
 * @return double 
 */
double Point3D::norm() const{
    return sqrt(m_x*m_x + m_y*m_y + m_z*m_z);
}

/** normalize
 * @brief Normalizes a vector
 */
void Point3D::normalize() {
    double norme = norm();
    m_x /= norme;
    m_y /= norme;
    m_z /= norme;
}

/**
 * @brief Computes the scalar product of 2 arrays
 * 
 * @param p1 a point
 * @param p2 a point
 * @return double 
 */
double scal(Point3D const& p1, Point3D const& p2) {
    return p1.m_x*p2.m_x + p1.m_y*p2.m_y + p1.m_z*p2.m_z;
}