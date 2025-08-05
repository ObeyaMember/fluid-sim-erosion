#ifndef _FLUID_SIMULATION_H_
#define _FLUID_SIMULATION_H_

#include "..//default_libs/default_libs.h"
#include "..//glad/glad.h"
#include "..//cglm/include/cglm/cglm.h"
/* #define STB_IMAGE_IMPLEMENTATION
#include "../../stb_image/stb_image.h" */
#include "..//GLFW/glfw3.h"
#include "../utilities/utilities.h"

struct fluid_sim_parameters_s {
    // BOUNDARIES
    vec3 bound_pos;
    vec3 bound_dims;
    float out_of_bounds_stiffness;
    float out_of_bounds_bounce_damp; // between 0 and 1

    // SPATIAL PARTITIONING
    int n_grid_cells_x;
    int n_grid_cells_y;
    int n_grid_cells_z;
    int n_grid_cells_total;
    int* grid_particle_cells; // holds cell idx of particle i at i
    int* grid_cells_num_particles_prefix_sums; 
    int* grid; // holds particle indices ordered by their cell indices
    
    
    // SPAWN BOX
    vec3 spawn_box_pos;
    vec3 spawn_box_dims;
    
    // PARTICLE PARAMETERS
    int n_particles;
    float particle_mass;
    float particle_radius;
    float grav_scale;
    float reference_density;
    float stiffness_k;
    float stiffness_gamma;
    float air_drag; // between 0 and 1

    // SIM STATE
    int is_running;
    float delta_time;
    
    vec3* positions;
    vec3* velocities;
    float* densities;
    float* pressures;
    
};

typedef struct fluid_sim_parameters_s fluid_sim_parameters; 

void pause_sim(GLFWwindow* window, fluid_sim_parameters* sim_params);

void setup_particle_positions_in_box(fluid_sim_parameters* sim_params);
void setup_particle_velocities(fluid_sim_parameters* sim_params);
void setup_particle_densities(fluid_sim_parameters* sim_params);
void setup_sim_grid(fluid_sim_parameters* sim_params);

void one_sim_step(fluid_sim_parameters* sim_params);

#endif