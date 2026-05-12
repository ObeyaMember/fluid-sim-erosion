#ifndef _FLUID_SIMULATION_H_
#define _FLUID_SIMULATION_H_

#include "..//default_libs/default_libs.h"
#include "..//glad/glad.h"
#include "..//cglm/include/cglm/cglm.h"
/* #define STB_IMAGE_IMPLEMENTATION
#include "../../stb_image/stb_image.h" */
#include "..//GLFW/glfw3.h"
#include "../utilities/utilities.h"

struct fluid_sim_parameters_s{
    int sim_id;

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
    int* grid_cells_num_partciles_count;
    int* grid_cells_num_particles_prefix_sums; 
    int* grid; // holds particle indices ordered by their cell indices
    
    
    // SPAWN BOX
    vec3 spawn_box_pos;
    vec3 spawn_box_dims;
    
    int positions_setup_mode; // 0: random in box (uses n_particles), 1: uniform specified number of particles per side
    int is_frozen; // != pause -> just no movement applied

    int n_particles_x; // used only for positions_setup_mode  = 1
    int n_particles_y; // used only for positions_setup_mode  = 1  
    int n_particles_z; // used only for positions_setup_mode  = 1  
    
    // PARTICLE PARAMETERS
    int n_particles; // used only for positions_setup_mode = 0
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
    
    struct fluid_sim_parameters_s* sim_to_copy;

};

typedef struct fluid_sim_parameters_s fluid_sim_parameters; 

void pause_sim(GLFWwindow* window, fluid_sim_parameters* sim_params);
void fluid_sim_setup(fluid_sim_parameters* sim_params);

void one_sim_step(fluid_sim_parameters* sim_params);
void one_sim_step_partitioned(fluid_sim_parameters* sim_params);

void fluid_sim_end(fluid_sim_parameters* sim_params);

#endif