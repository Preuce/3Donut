#include "const.hpp"

#include <limits>
#include <iostream>
#include <thread>

//note concernant le repère, x vers la droite, y vers le haut, z loin de l'écran

//Rotation angles
double phi = 0;/*angle on the x axis*/
double alpha = 0; /*angle on the y axis*/
double ome = 0; /*angle on the z axis*/

unsigned int id_phi = 0;/*angle on the x axis*/
unsigned int id_alpha = 0; /*angle on the y axis*/
unsigned int id_ome = 0; /*angle on the z axis*/

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
inline void compute_rot(){
    for(unsigned long i = 0; i < donut_mesh.size(); i++){
        Point3D point = donut_mesh[i];

        //Applying the rotations
        point.Rot_x(s_phi[id_phi], c_phi[id_phi]).Rot_y(s_alpha[id_alpha], c_alpha[id_alpha]).Rot_z(s_ome[id_ome], c_ome[id_ome]);

        //yeah writing annoys me
        auto const& [x, y, z] = point;

        if(z < DEPTH[((RP - (int) y)*ZOOM)*GRAPH_DIM + ((int) x + RP)*ZOOM]){
            //Point in the inner ring
            Point3D inner_point = inner_ring[i/size_the];

            //Rotation
            inner_point.Rot_x(s_phi[id_phi], c_phi[id_phi]).Rot_y(s_alpha[id_alpha], c_alpha[id_alpha]).Rot_z(s_ome[id_ome], c_ome[id_ome]);

            //Orientation vector of the point in the space
            Point3D orient_vector = point - inner_point;
            
            //Normalization
            orient_vector.normalize();
            
            //Illumination, scalar product between the illumination vector I and the normal at x, y, z
            //result between -1 et 1, then within MAP size 
            int illum = std::min((int) (scal(orient_vector, I)*MAP_size*SATURATION), MAP_size);
            //If the scalar product is positive (normal in the orientation of I)
            if(illum >= 0){
                illum = 1;
            }else{
                //If the scalar product is negative (normal opposite to I)
                illum = std::min(-illum, MAP_size);
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

inline void update_id(unsigned int &id, unsigned long const& size){
    id = (id+1)%size;
}

inline void update_angles_id(){
    update_id(id_phi, size_phi);
    update_id(id_alpha, size_alpha);
    update_id(id_ome, size_ome);
}

inline void update_angle(double& angle, double const& pre){
    angle = fmod(angle+pre, 2*M_PI);
}

/**
 * @brief updates the values of the rotation angles
 * the value is updated so that the increment per second corresponds to the value specified in params.hpp
 */
inline void update_angles(){
    update_angle(phi, pre_phi); //x axis
    update_angle(alpha, pre_alpha); //y axis
    update_angle(ome, pre_ome); //z axis
}

int main(int argc, char* argv[]){
    I.normalize();

    make_donut_mesh();

    const auto GRAPH_begin = std::begin(GRAPH), GRAPH_end = std::end(GRAPH);

    const auto DEPTH_begin = std::begin(DEPTH), DEPTH_end = std::end(DEPTH);

    while(true){
        std::fill(GRAPH_begin, GRAPH_end, 0);
        std::fill(DEPTH_begin, DEPTH_end, std::numeric_limits<int>::max()); //arbitrary value > max(z) 

        compute_rot();
        
        printf("phi : %d    alpha : %d    ome : %d\n", ((int) (phi*180/M_PI))%360, ((int) (alpha*180/M_PI)%360), ((int) (ome*180/M_PI))%360);

        // print_GRAPH();
        // print_DEPTH();
        // print_values();
        print_light();

        update_angles_id();
        update_angles(); //unnecessary, for angle display only (why not)

        std::this_thread::sleep_for(FRAME_DURATION);
    }
}