#ifndef _FLUID_SIMULATION_RENDER_H_
#define _FLUID_SIMULATION_RENDER_H_

#include "..//default_libs/default_libs.h"
#include "..//glad/glad.h"
#include "..//cglm/include/cglm/cglm.h"
/* #define STB_IMAGE_IMPLEMENTATION
#include "../../stb_image/stb_image.h" */
#include "..//GLFW/glfw3.h"
#include "../utilities/utilities.h"

#include "../../libraries/fluid_simulation/fluid_simulation.h"

struct fluid_render_parameters_s {

    //                                                  SHADERS AND SHADER PROGRAMS
    // particles
    unsigned int particle_vertex_shader, particle_fragment_shader;
    unsigned int particle_shader_program;

    // sim bounding
    unsigned int bounding_vertex_shader, bounding_fragment_shader;
    unsigned int bounding_shader_program;

    // sim grid
    unsigned int grid_vertex_shader, grid_fragment_shader;
    unsigned int grid_shader_program;

    //                                                               DATA
    // one particle shape
    float render_radius;

    float* fluid_particle_vertices; 
    int fluid_particle_n_vertices;

    unsigned int* fluid_particle_indices;
    int fluid_particle_n_indices;

    // sim bounding shape
    float* fluid_sim_bounding_vertices;
    int fluid_sim_bounding_n_vertices;
    
    unsigned int* fluid_sim_bounding_indices;
    int fluid_sim_bounding_n_indices;

    // sim grid cell shape
    float* fluid_sim_grid_cell_vertices;
    int fluid_sim_grid_cell_n_vertices;

    unsigned int* fluid_sim_grid_cell_indices;
    int fluid_sim_grid_cell_n_indices;

    // sim grid cell
    vec3 fluid_sim_grid_cell_dims;

    //                                                              BUFFERS
    // vao
    unsigned int VAO;
    
    // one particle
    unsigned int fluid_particle_vertices_VBO;
    unsigned int fluid_particle_IBO;
    
    // all particles
    unsigned int fluid_particles_pos_VBO;
    unsigned int fluid_particles_densities_VBO;
    unsigned int fluid_particles_pressures_VBO;

    // sim bounding
    unsigned int fluid_sim_bounding_vertices_VBO;
    unsigned int fluid_sim_bounding_IBO;

    // sim grid cell shape
    unsigned int fluid_sim_grid_cell_vertices_VBO;
    unsigned int fluid_sim_grid_cell_IBO;

    // whole sim grid
    unsigned int fluid_sim_grid_prefix_sums_VBO;
};

typedef struct fluid_render_parameters_s fluid_render_paramters; 

void fluid_renderer_setup(fluid_render_paramters* render_params, fluid_sim_parameters* sim_params);
void fluid_renderer_loop_draw_bounding(camera_3d* camera, fluid_render_paramters* render_params, fluid_sim_parameters* sim_params);
void fluid_renderer_loop_draw_fluid_particles(camera_3d* camera, fluid_render_paramters* render_params, fluid_sim_parameters* sim_params);
void fluid_renderer_loop_draw_sim_grid(camera_3d* camera, fluid_render_paramters* render_params, fluid_sim_parameters* sim_params);

#endif