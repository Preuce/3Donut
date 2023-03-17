#ifndef PARAMS_HPP
#define PARAMS_HPP
#include "points.hpp"

#include <string>
#include <vector>

//Donut
constexpr int R = 18; //internal ring radius
constexpr int r = 8; //external ring radius

//Display
constexpr unsigned int ZOOM = 1; //zoom
constexpr double FPS = 30; //frames per second

//Rotation speed in rad per second
constexpr double rot_phi = 0.8; //x axis
constexpr double rot_alpha = 0.4; //y axis
constexpr double rot_ome = 0.2; //z axis

//Illumination
//character map based on illumination level
//note : the first character corresponds to empty space
const std::vector<std::string> MAP {"  ", "..", "--", "::", ";;", "//", "##", "$$"};
Point3D I = {0, 0, 1}; //illumination vector
constexpr double INTENSITY = 1.0; //light intensity

#endif