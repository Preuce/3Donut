# Donut
The code displays a rotating 3D donut with lighting... That's pretty much it  

You can compile it using compile.sh, then run the 'Donut' executable.

# File Structure

1. points
    * class implementing 3D points, and some functions used for computations (most notably the rotation functions)
2. params
    * file to define parameters in
3. const
    * mesh definition and some other stuff that won't move during the execution
4. main
    * main code
    * Contains the mesh rotation code
    * Contains 4 display functions
        * print_GRAPH : displays the raw donut (no lighting)
        * print_DEPTH : displays the content of DEPTH (z axis values) with color-coding
        * print_values : displays the content of GRAPH (numerical values)
        * print_light : displays the donut with proper lighting, mapping the values in GRAPH onto the ones defines in MAP

# Parameters
You can tweak the parameters in params.hpp.
* R and r correspond to the inner ring, and the outer ring of the donut respectively
* ZOOM corresponds to the amount of zoom on the donut. Makes it bigger without really increasing the computing time
* FPS corresponds to amount of frame per second
* rot_phi, rot_alpha and rot_ome correspond to the rotation speed on the x, y, and z axis respectively, in rad per second.
* MAP corresponds to the caracters to be used as lighting gradient
* I corresponds to the illumination vector, determines the direction of the light
* SATURATION corresponds to the saturation of the light

# Implementation
The coordinate system we consider is the following :
* The x-axis goes right, the y-axis goes up, and the z-axis goes into the screen, away from the user.  
* phi is the rotation angle over the x-axis. Goes from z to y.
* alpha is the rotation angle over the y-axis. Goes from x to z. 
* omega is the rotation angle over the z-axis. Goes from x to y.

The base donut mesh is computed only once in const.hpp (couldn't figure out how to compute it at compile time :penguin:), as a list of 3D points.  
The Donut and the depth map are represented as 2D grids, where each point is mapped to the corresponding (x, y) coordinates, DEPTH only contains the z value for each point placed in GRAPH (and an arbitrarily large value for empty spots).  
A point is placed in GRAPH if and only if its z value is smaller than the one currently in DEPTH.  

The main code iterates over the list of point in the mesh, and rotates each points based on the 3 angles phi, alpha and ome(ga).  
The sine and cosine values for each relevant angle are computed at the beginning of execution.  
It then computes the scalar product between the illumination vector I and the normal at each points in order to determine the light level.  

# TODO
* The rotation speed is not fully consistent between different framerates. It might be due to values being improperly rounded  
* User inputs are a pain, they could allow proper termination of the program, as well as switching from one view to another  
* We could compute all ```size_phi*size_alpha*size_ome``` rotation matrices (or frames...) at the beginning of execution  
* The light is a bit f*cked, it doesn't behave very naturally, that's due to the fact that I take the scalar product of the illumination vector and the normal at each points, so a point directly under the light but facing slightly away will be a lot darker than it should. It also doesn't take occlusion into account, the light traverses the mesh