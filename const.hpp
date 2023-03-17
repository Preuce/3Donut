#ifndef CONST_HPP
#define CONST_HPP
#include "params.hpp"

#include <algorithm>
#include <chrono>

constexpr int RP = (R+r); //max radius, used as offset to place elements in tables
constexpr int GRAPH_DIM = ((RP * 2 + 1)*ZOOM); //size of tables storing the displays

constexpr std::chrono::microseconds FRAME_DURATION((int) (1000000/std::max(0.01, FPS))); //Pause in between each frame, in microsecond

/*----------Pre-computations----------*/

//Precision, used at angle increment
constexpr double pre_phi = rot_phi/FPS;
constexpr double pre_alpha = rot_alpha/FPS;
constexpr double pre_ome = rot_ome/FPS;

//Computing the amount of value we will need
constexpr unsigned long size_phi = (unsigned long) (2*M_PI/pre_phi);
constexpr unsigned long size_alpha = (unsigned long) (2*M_PI/pre_alpha);
constexpr unsigned long size_ome = (unsigned long) (2*M_PI/pre_ome);

/**
 * @brief returns an array of length size and applies a function at every index
*/
template 
<typename T>
T pre_compute(int size, double pre, double (*func)(double)){
    T arr;
    for(int i = 0; i < size; i++){
        arr[i] = func(i*pre);
    }

    return arr;
}

//Precomputed array of sine and cos
inline const auto s_phi = pre_compute<std::array<double, size_phi>>(size_phi, pre_phi, &sin);
inline const auto c_phi = pre_compute<std::array<double, size_phi>>(size_phi, pre_phi, &cos);

inline const auto s_alpha = pre_compute<std::array<double, size_alpha>>(size_alpha, pre_alpha, &sin);
inline const auto c_alpha = pre_compute<std::array<double, size_alpha>>(size_alpha, pre_alpha, &cos);

inline const auto s_ome = pre_compute<std::array<double, size_ome>>(size_ome, pre_ome, &sin);
inline const auto c_ome = pre_compute<std::array<double, size_ome>>(size_ome, pre_ome, &cos);

/*----------Donut generation----------*/
//the : angle in the external ring
//psy : angle on the y axis

//Precision, used at mesh generation
constexpr double pre_psy = 0.3/(double) std::max(1, R);
constexpr double pre_the = 0.3/(double) std::max(1, r);

constexpr unsigned long size_psy = (unsigned long) (2*M_PI/pre_psy);
constexpr unsigned long size_the = (unsigned long) (2*M_PI/pre_the);

//meshes
inline std::array<Point3D, size_psy*size_the> donut_mesh;
inline std::array<Point3D, size_psy> inner_ring;

/**
 * @brief Populate the meshes with points
 * 
 */
void make_donut_mesh(){

    double cos_the[size_the];
    double sin_the[size_the];

    //pre-computation of every relevent sin and cos of theta
    for(unsigned long i = 0; i < size_the; i++){
        const double the = i*pre_the;
        cos_the[i] = cos(the);
        sin_the[i] = sin(the);
    }

    double psy[size_psy];
    for(unsigned long i = 0; i < size_psy; i++){
        psy[i] = i*pre_psy;
    }

    //Could use the symmetry to shorten the loops
    for(unsigned long k = 0; k < size_psy; k++){
        const double cos_psy = cos(psy[k]);
        const double sin_psy = sin(psy[k]);

        inner_ring[k] = {R*cos_psy, R*sin_psy, 0};

        for(unsigned long i = 0; i < size_the; i++){
            donut_mesh[k*size_the + i] = {R*cos_psy + r*cos_the[i]*cos_psy, R*sin_psy + r*sin_psy*cos_the[i], r*sin_the[i]};
        }
    }
}

#endif