#include "const.hpp"

#include <limits>
#include <iostream>
#include <thread>
#include <chrono>
#include <math.h>

//note concernant le repère, x vers la droite, y vers le haut, z loin de l'écran


//Rotation angles
double phi = 0;/*angle on the x axis*/
double alpha = 0; /*angle on the y axis*/
double ome = 0; /*angle on the z axis*/

int GRAPH[GRAPH_DIM*GRAPH_DIM] {0};
int DEPTH[GRAPH_DIM*GRAPH_DIM] {RP+10};

/**
 * @brief Prints the donut
 * 
 */
void print_GRAPH(){
    for(int i = 0; i < GRAPH_DIM; i++){
        for(int j = 0; j < GRAPH_DIM; j++){
            if(GRAPH[i*GRAPH_DIM + j] >= 1){
                printf("##");
            }else{
                printf("__");
            }
        }
        printf("\n");
    }
}

/**
 * @brief Prints a depth map
 * Blue : positive values >= 10
 * Cyan : positive values < 10
 * Orange : negative values > -10
 * Red : negative values <= -10
 */
void print_DEPTH(){
    for(int i = 0; i < GRAPH_DIM; i++){
        for(int j = 0; j < GRAPH_DIM; j++){
            if(DEPTH[i*GRAPH_DIM+j] < std::numeric_limits<int>::max()){

                if(DEPTH[i*GRAPH_DIM+j] >= 10){
                    printf("\033[34m%d\033[0m", abs(DEPTH[i*GRAPH_DIM + j]));
                }else if(DEPTH[i*GRAPH_DIM+j] >= 0){
                    printf("\033[36m%d%d\033[0m", abs(DEPTH[i*GRAPH_DIM + j]), abs(DEPTH[i*GRAPH_DIM + j]));
                }else if(DEPTH[i*GRAPH_DIM+j] >= -9){
                    printf("\033[33m%d%d\033[0m", abs(DEPTH[i*GRAPH_DIM + j]), abs(DEPTH[i*GRAPH_DIM + j]));
                }else{
                    printf("\033[31m%d\033[0m", abs(DEPTH[i*GRAPH_DIM + j]));
                }
            }else{
                printf("  ");
            }
        }
        printf("\n");
    }
}

/**
 * @brief Prints the light intensity using numerical values
 * 
 */
void print_values(){
    for(int i = 0; i < GRAPH_DIM; i++){
        for(int j = 0; j < GRAPH_DIM; j++){
            if(GRAPH[i*GRAPH_DIM + j] >= 1){
                printf("%d%d", GRAPH[i*GRAPH_DIM + j], GRAPH[i*GRAPH_DIM + j]);
            }else{
                printf("00");
            }
        }
        printf("\n");
    }
}

/**
 * @brief Prints the light intensity as characters
 * 
 */
void print_light(){
    for(int i = 0; i < GRAPH_DIM; i++){
        for(int j = 0; j < GRAPH_DIM; j++){
            if(GRAPH[i*GRAPH_DIM + j] >= 0)
                printf("%s", MAP[GRAPH[i*GRAPH_DIM + j]].c_str());
        }
        printf("\n");
    }
}

/**
 * @brief Computes the coordinates of each point based on the 5 different angles
 * Store the depths and illumination in DEPTH and GRAPH
 */
void compute_rot(){
    for(unsigned long i = 0; i < donut_mesh.size(); i++){
        Point3D point = donut_mesh[i];

        //Applying the rotations
        point.Rot_x(phi).Rot_y(alpha).Rot_z(ome);

        //yeah writing annoys me
        auto const& [x, y, z] = point;

        if(z < DEPTH[((RP - (int) y)*ZOOM)*GRAPH_DIM + ((int) x + RP)*ZOOM]){
            //Point in the inner ring
            Point3D inner_point = inner_ring[i/(donut_mesh.size()/size_psy)];
            //Rotation
            inner_point.Rot_x(phi).Rot_y(alpha).Rot_z(ome);

            //Orientation vector of the point in the space
            Point3D orient_vector = point - inner_point;
            
            //Normalization
            orient_vector.normalize();
            
            //Illumination, scalar product between the illumination vector I and the normal at x, y, z
            //result between -1 et 1, then within MAP size 
            int illum = std::min((int) (scal(orient_vector, I)*MAP_last*INTENSITY), MAP_last);
            //If the scalar product is positive (normal in the orientation of I)
            if(illum >= 0){
                illum = 1;
            }else{
                //If the scalar product is negative (normal opposite to I)
                illum = std::min(-illum, MAP_last);
            }

            int depth = std::max(-RP, std::min((int) z, RP));

            for(unsigned long i = 0; i < ZOOM; i++){
                int idx = ((int) x + RP)*ZOOM + i;
                for(unsigned long j = 0; j < ZOOM; j++){
                    int idy = (RP - (int) y)*ZOOM - j;
                    GRAPH[idy*GRAPH_DIM + idx] = illum;
                    DEPTH[idy*GRAPH_DIM + idx] = depth;
            }}
        }
    }
}

/**
 * @brief Computes the coordinates of each point based on the 5 different angles
 * Store the depths and illumination in DEPTH and GRAPH
 * Recompute the entire donut at each call
 */
void compute_rot_ineff(){

    for(double psy = 0.0; psy <= 2*M_PI; psy+=pre_psy){
        for(double the = 0.0; the <= 2*M_PI; the+=pre_the){      

            //Main point
            Point3D point_rot {X, Y, Z};

            //Applying the rotations
            point_rot.Rot_x(phi).Rot_y(alpha).Rot_z(ome);

            //yeah writing annoys me
            auto const& [x, y, z] = point_rot;
            
            //Remark : I prefer doing some weird shenanigans here with the coordinates, rather than "lying" in the display function.
            //in order to respect the usual norms (x + to the right, y + to the top) the indices in GRAPH and DEPTH are as follows  
            if(z < DEPTH[((RP - (int) y)*ZOOM)*GRAPH_DIM + ((int) x + RP)*ZOOM]){

                //Point in the inner ring
                Point3D inner_point {Rx, Ry, Rz};
                //Rotation
                inner_point.Rot_x(phi).Rot_y(alpha).Rot_z(ome);

                //Orientation vector of the point in the space
                Point3D orient_vector = point_rot - inner_point;
                
                //Normalization
                orient_vector.normalize();
                
                //Illumination, scalar product between the illumination vector I and the normal at x, y, z
                //result between -1 et 1, then within MAP size 
                int illum = std::min((int) (scal(orient_vector, I)*MAP_last*INTENSITY), MAP_last);
                //If the scalar product is positive (normal in the orientation of I)
                if(illum >= 0){
                    illum = 1;
                }else{
                    //If the scalar product is negative (normal opposite to I)
                    illum = std::min(-illum, MAP_last);
                }

                int depth = std::max(-RP, std::min((int) z, RP));

                for(unsigned long i = 0; i < ZOOM; i++){
                    int idx = ((int) x + RP)*ZOOM + i;
                    for(unsigned long j = 0; j < ZOOM; j++){
                        int idy = (RP - (int) y)*ZOOM - j;
                        GRAPH[idy*GRAPH_DIM + idx] = illum;
                        DEPTH[idy*GRAPH_DIM + idx] = depth;
                }}
            }
        }
    }
}

/**
 * @brief updates the values of the rotation angles
 * the value is updated so that the increment per second corresponds to the value specified in params.hpp
 */
void update_angles(){
    phi+=rot_phi/FPS; //x axis
    alpha+=rot_alpha/FPS; //y axis
    ome+=rot_ome/FPS; //z axis
}

int main(int argc, char* argv[]){
    I.normalize();

    const auto GRAPH_begin = std::begin(GRAPH), GRAPH_end = std::end(GRAPH);

    const auto DEPTH_begin = std::begin(DEPTH), DEPTH_end = std::end(DEPTH);
    
    while(true){

        std::fill(GRAPH_begin, GRAPH_end, 0);
        std::fill(DEPTH_begin, DEPTH_end, std::numeric_limits<int>::max()); //arbitrary value > max(z) 

        compute_rot();
        // compute_rot_ineff();

        printf("phi : %d    alpha : %d    ome : %d\n", ((int) (phi*180/M_PI))%360, ((int) (alpha*180/M_PI)%360), ((int) (ome*180/M_PI))%360);

        // print_GRAPH();
        // print_DEPTH();
        // print_values();
        print_light();

        update_angles();

        std::this_thread::sleep_for(std::chrono::microseconds((int) (1000000/FPS)));
    }
}