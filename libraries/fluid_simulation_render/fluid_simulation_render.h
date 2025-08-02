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

void fluid_renderer_pass_one_particle_data(float* particle_vertices, int particle_n_vertices, unsigned int* particle_indices, int particle_n_indices);
void fluid_renderer_pass_bounding_data(float* sim_bounding_vertices, int sim_bounding_n_vertices, unsigned int* sim_bounding_indices, int sim_bounding_n_indices);
void fluid_renderer_setup(fluid_sim_parameters* sim_params);
void fluid_renderer_loop_draw_bounding(mat4 view, mat4 projection, fluid_sim_parameters* sim_params);
void fluid_renderer_loop_draw_fluid_particles(mat4 view, mat4 projection, float particle_render_radius, fluid_sim_parameters* sim_params);

#endif