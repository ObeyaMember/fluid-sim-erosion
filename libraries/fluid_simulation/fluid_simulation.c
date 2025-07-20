#include "fluid_simulation.h"

static vec3 x_axis = {1,0,0};
static vec3 y_axis = {0,1,0};
static vec3 z_axis = {0,0,1};


void setup_particle_positions_in_box(vec3* positions, int n_particles, vec3 spawn_box_pos, vec3 spawn_box_dims){
    
    float length = spawn_box_dims[0];
    float height = spawn_box_dims[1];
    float width = spawn_box_dims[2];

    float MAX = RAND_MAX;
    float x, y, z;
    vec3 start_pos = {-length/2.0, -height/2.0, -width/2.0};

    for (int i = 0; i < n_particles; i += 1){
        
        x = (rand() / MAX) * length + spawn_box_pos[0];
        y = (rand() / MAX) * height + spawn_box_pos[1];
        z = (rand() / MAX) * width + spawn_box_pos[2];
        
        vec3 to_add_pos = {x, y, z};
        glm_vec3_add(start_pos, to_add_pos, positions[i]);
        

    }

}

void setup_particle_velocities(vec3* velocities, int n_particles){
    for (int i = 0; i < n_particles; i += 1){
        vec3 start_vel = {20, 20, 0};
        glm_vec3_copy(start_vel, velocities[i]);
        
    }
}

void one_sim_step(vec3* positions, vec3* velocities, int n_particles, vec3 bound_pos, vec3 box_dims, float delta_time){
    
    float grav_scale = 10;

    // NO SPACE PARTITIONING
    for (int i = 0; i < n_particles; i += 1){

        vec3 total_force;
        vec3 accel;
        glm_vec3_zero(total_force);
        glm_vec3_zero(accel);
        
        // APPLY GRAVITY
        vec3 grav_accel;
        glm_vec3_scale(y_axis, -grav_scale, grav_accel);
        glm_vec3_add(accel, grav_accel, accel);
        
        /* for (int j = 0; j < n_particles; j += 1){
            if (j != i){
                
            }
        } */

        // CALCULATE NEW ACCEL


        // CALCULATE NEW VELOCITY
        glm_vec3_scale(accel, delta_time, accel);
        glm_vec3_add(velocities[i], accel, velocities[i]);
        
        // CALCULATE NEW POSITIONS
        vec3 delta_vel;
        glm_vec3_scale(velocities[i], delta_time, delta_vel);
        glm_vec3_add(positions[i], delta_vel, positions[i]);


    }

    //printf("velocities[0][1]: %f\n", velocities[0][1]);

}