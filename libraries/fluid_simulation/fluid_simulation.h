#ifndef _FLUID_SIMULATION_H_
#define _FLUID_SIMULATION_H_

#include "..//default_libs/default_libs.h"
#include "..//glad/glad.h"
#include "..//cglm/include/cglm/cglm.h"
/* #define STB_IMAGE_IMPLEMENTATION
#include "../../stb_image/stb_image.h" */
#include "..//GLFW/glfw3.h"
#include "../utilities/utilities.h"

void setup_particle_positions_in_box(vec3* positions, int n_particles, vec3 spawn_box_pos, vec3 spawn_box_dims);
void setup_particle_velocities(vec3* velocities, int n_particles);
void one_sim_step(vec3* positions, vec3* velocities, int n_particles, vec3 bound_pos, vec3 box_dims, float delta_time);

#endif