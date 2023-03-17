#ifndef CONST_HPP
#define CONST_HPP
#include "params.hpp"

constexpr int RP = (R+r); //max radius, used as offset to place elements in tables
constexpr int GRAPH_DIM = ((RP * 2 + 1)*ZOOM); //size of tables storing the displays

int MAP_last = MAP.size()-1; //don't pay attention to that


/*Donut generation :
the : angle in the external ring
psy : angle on the y axis
*/

//Precision, used at mesh generation
constexpr double pre_psy = 0.3/(double) std::max(1, R);
constexpr double pre_the = 0.3/(double) std::max(1, r);

//coordinates given psy and theta (backward facing donut)
#define Rx (R*cos(psy))
#define rx (r*cos(the)*cos(psy))

#define Ry (R*sin(psy))
#define ry (r*sin(psy)*cos(the))

#define Rz (0)
#define rz (r*sin(the))

#define X (Rx + rx)
#define Y (Ry + ry)
#define Z (Rz + rz)

/*TODO
Generate the base mesh for the donut at compile time
Compute cos and sin for phi, alpha and ome at compile time
*/

const unsigned long size_psy = (unsigned long) (2*M_PI/pre_psy);
const unsigned long size_the = (unsigned long) (2*M_PI/pre_the);

/**
 * @brief Creates the main mesh for the donut
 * 
 * @return a vector
 */
std::vector<Point3D> make_donut_mesh(){

    std::vector<Point3D> mesh;

    //Could use the symmetry to shorten the loops
    for(double psy = 0.0; psy <= 2*M_PI; psy+=pre_psy){
        for(double the = 0.0; the <= 2*M_PI; the+=pre_the){
            //convert the and psy into an index
            // mesh[psy*size_the + the] = Point3D {X, Y, Z};
            mesh.push_back({X, Y, Z});
        }
    }
    return mesh;
}

const auto donut_mesh = make_donut_mesh();

std::vector<Point3D> make_inner_ring(){
    std::vector<Point3D> mesh;
    
    //Could use the symmetry to shorten the loop
    for(double psy = 0.0; psy <= 2*M_PI; psy+=pre_psy){
        // mesh[psy/pre_psy] = Point3D {R*cos(psy), R*sin(psy), 0};
        mesh.push_back({R*cos(psy), R*sin(psy), 0});
    }

    return mesh;
}

const auto inner_ring = make_inner_ring(); 
#endif