#include "fluid_simulation.h"

static vec3 x_axis = {1,0,0};
static vec3 y_axis = {0,1,0};
static vec3 z_axis = {0,0,1};


void setup_particle_positions_in_box(fluid_sim_parameters* sim_params){
    vec3 spawn_box_pos;
    vec3 spawn_box_dims;
    glm_vec3_copy(sim_params->spawn_box_pos, spawn_box_pos);
    glm_vec3_copy(sim_params->spawn_box_dims, spawn_box_dims);

    float length = spawn_box_dims[0];
    float height = spawn_box_dims[1];
    float width = spawn_box_dims[2];

    float MAX = RAND_MAX;
    float x, y, z;
    vec3 start_pos = {-length/2.0, -height/2.0, -width/2.0};

    for (int i = 0; i < sim_params->n_particles; i += 1){
        
        x = (rand() / MAX) * length + spawn_box_pos[0];
        y = (rand() / MAX) * height + spawn_box_pos[1];
        z = (rand() / MAX) * width + spawn_box_pos[2];
        
        vec3 to_add_pos = {x, y, z};
        glm_vec3_add(start_pos, to_add_pos, sim_params->positions[i]);
        
    }

}

void setup_particle_velocities(fluid_sim_parameters* sim_params){
    for (int i = 0; i < sim_params->n_particles; i += 1){
        vec3 start_vel = {0,0,0};
        glm_vec3_copy(start_vel, sim_params->velocities[i]);
        
    }
}

void setup_particle_densities(fluid_sim_parameters* sim_params){
    for (int i = 0; i < sim_params->n_particles; i += 1){
        sim_params->densities[i] = sim_params->reference_density;
    }
}

// NO NEED TO SETUP PRESSURES

// APPLY BOUNCE FRICTION (NO CLUE IF THIS IS HOW IT WORKS)
static void get_bounce_damp_force(fluid_sim_parameters* sim_params, int particle_idx, vec3 dest_force){
    vec3 res_force;
    
    vec3 p_vel;
    glm_vec3_copy(sim_params->velocities[particle_idx], p_vel);
    
    glm_vec3_scale(p_vel, -(sim_params->out_of_bounds_bounce_damp) * sim_params->particle_mass * (1 / sim_params->delta_time), res_force);

    glm_vec3_copy(res_force, dest_force);
}

static void get_out_of_bounds_force(fluid_sim_parameters* sim_params, int particle_idx, vec3 dest_force){
    
    vec3 res_force = {0,0,0};
    
    float k = sim_params->out_of_bounds_stiffness;
    
    vec3 p_pos;
    glm_vec3_copy(sim_params->positions[particle_idx], p_pos);

    
    
    vec3 bound_pos;
    glm_vec3_copy(sim_params->bound_pos, bound_pos);
    
    vec3 bound_dims;
    glm_vec3_copy(sim_params->bound_dims, bound_dims);
    
    float dist_to_right_bound = p_pos[0] - (bound_pos[0] + bound_dims[0]/2.0);
    float dist_to_left_bound = (bound_pos[0] - bound_dims[0]/2.0) - p_pos[0];

    if (dist_to_right_bound > 0){
        
        // BOUNCE OFF WALL
        vec3 bound_bounce_force = {-sim_params->out_of_bounds_stiffness * dist_to_right_bound, 0, 0};
        glm_vec3_add(res_force, bound_bounce_force, res_force);

        // DAMPENING
        vec3 bound_bounce_fric_force;
        get_bounce_damp_force(sim_params, particle_idx, bound_bounce_fric_force);
        glm_vec3_add(res_force, bound_bounce_fric_force, res_force);
    }
    if (dist_to_left_bound > 0){
        
        // BOUNCE OFF WALL
        vec3 bound_bounce_force = {sim_params->out_of_bounds_stiffness * dist_to_left_bound, 0, 0};
        glm_vec3_add(res_force, bound_bounce_force, res_force);

        // DAMPENING
        vec3 bound_bounce_fric_force;
        get_bounce_damp_force(sim_params, particle_idx, bound_bounce_fric_force);
        glm_vec3_add(res_force, bound_bounce_fric_force, res_force);
    }
    
    float dist_to_up_bound = p_pos[1] - (bound_pos[1] + bound_dims[1]/2.0);
    float dist_to_down_bound = (bound_pos[1] - bound_dims[1]/2.0) - p_pos[1];
    
    if (dist_to_up_bound > 0){
        
        // BOUNCE OFF WALL
        vec3 bound_bounce_force = {0, -sim_params->out_of_bounds_stiffness * dist_to_up_bound, 0};
        glm_vec3_add(res_force, bound_bounce_force, res_force);

        // DAMPENING
        vec3 bound_bounce_fric_force;
        get_bounce_damp_force(sim_params, particle_idx, bound_bounce_fric_force);
        glm_vec3_add(res_force, bound_bounce_fric_force, res_force);
    }
    if (dist_to_down_bound > 0){
        
        // BOUNCE OFF WALL
        vec3 bound_bounce_force = {0, sim_params->out_of_bounds_stiffness * dist_to_down_bound, 0};
        glm_vec3_add(res_force, bound_bounce_force, res_force);

        // DAMPENING
        vec3 bound_bounce_fric_force;
        get_bounce_damp_force(sim_params, particle_idx, bound_bounce_fric_force);
        glm_vec3_add(res_force, bound_bounce_fric_force, res_force);

    }

    float dist_to_back_bound = p_pos[2] - (bound_pos[2] + bound_dims[2]/2.0);
    float dist_to_front_bound = (bound_pos[2] - bound_dims[2]/2.0) - p_pos[2];

    if (dist_to_back_bound > 0){
        
        // BOUNCE OFF WALL
        vec3 bound_bounce_force = {0, 0, -sim_params->out_of_bounds_stiffness * dist_to_back_bound};
        glm_vec3_add(res_force, bound_bounce_force, res_force);

        // DAMPENING
        vec3 bound_bounce_fric_force;
        get_bounce_damp_force(sim_params, particle_idx, bound_bounce_fric_force);
        glm_vec3_add(res_force, bound_bounce_fric_force, res_force);
    }
    if (dist_to_front_bound > 0){
        
        // BOUNCE OFF WALL
        vec3 bound_bounce_force = {0, 0, sim_params->out_of_bounds_stiffness * dist_to_front_bound};
        glm_vec3_add(res_force, bound_bounce_force, res_force);

        // DAMPENING
        vec3 bound_bounce_fric_force;
        get_bounce_damp_force(sim_params, particle_idx, bound_bounce_fric_force);
        glm_vec3_add(res_force, bound_bounce_fric_force, res_force);

        
    }

    // APPLY TOTAL BOUNCE + DAMP FORCE

    glm_vec3_copy(res_force, dest_force);

}

void pause_sim(GLFWwindow* window, fluid_sim_parameters* sim_params){
    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS){
        if (sim_params->is_running == 1){
            sim_params->is_running = 0;
        }else {
            sim_params->is_running = 1;
        }
    }
}

float clamp_pairwise_dist(float dist){
    return glm_clamp(dist, 1, 100000000);
}

static float smoothing_kernel_linear(fluid_sim_parameters* sim_params, float dist_to_center){
    return glm_clamp(1.0 - (1.0 / sim_params->particle_radius) * abs(dist_to_center), 0.0, 1.0);
}

static float smoothing_kernel_linear_deriv(fluid_sim_parameters* sim_params, float dist_to_center){
    
    if (dist_to_center <= sim_params->particle_radius){
        return -(1.0 / sim_params->particle_radius);
    }
    else {
        return 0;
    }
}

static void update_sim_densities(fluid_sim_parameters* sim_params){

    for (int particle_idx = 0; particle_idx < sim_params->n_particles - 1; particle_idx += 1){
        
        sim_params->densities[particle_idx] = sim_params->particle_mass * smoothing_kernel_linear(sim_params, 0);
        
        for (int i = particle_idx+1; i < sim_params->n_particles; i += 1){
            
            float pairwise_dist = glm_vec3_distance(sim_params->positions[particle_idx], sim_params->positions[i]);
            pairwise_dist = clamp_pairwise_dist(pairwise_dist);
            
            float pairwise_density = sim_params->particle_mass * smoothing_kernel_linear(sim_params, pairwise_dist);
            sim_params->densities[particle_idx] += pairwise_density;
            sim_params->densities[i] += pairwise_density;
        
        }

        

    }

    //printf("density[0]: %f\n", sim_params->densities[0]);
    //printf("density[1]: %f\n", sim_params->densities[1]);
    
}

static void update_sim_pressure_at_particle(fluid_sim_parameters* sim_params, int particle_idx){
    
    float k = sim_params->stiffness_k;
    float gamma = sim_params->stiffness_gamma;
    float ref_density = sim_params->reference_density;
    float idx_density = sim_params->densities[particle_idx];
    
    sim_params->pressures[particle_idx] = (k / gamma) * ref_density * (powf(idx_density / ref_density, gamma) - 1.0);
    //printf("pressure[%d]: %f\n", particle_idx, sim_params->pressures[particle_idx]);
}

static void get_pressure_force(fluid_sim_parameters* sim_params, int particle_idx, vec3 dest_force){
    
    vec3 res_force = {0,0,0};
    for (int i = 0; i < sim_params->n_particles; i += 1){
        if (i != particle_idx){
            
            vec3 pairwise_force;
            
            // CALCULATE KERNEL GRADIENT
            vec3 kernel_grad;
            vec3 dir_idx_to_i;
            float pairwise_dist = glm_vec3_distance(sim_params->positions[particle_idx], sim_params->positions[i]);
            pairwise_dist = clamp_pairwise_dist(pairwise_dist);

            float kernel_deriv = smoothing_kernel_linear_deriv(sim_params, pairwise_dist);
        
            glm_vec3_sub(sim_params->positions[particle_idx], sim_params->positions[i], dir_idx_to_i);
            glm_vec3_normalize(dir_idx_to_i);
            glm_vec3_scale(dir_idx_to_i, kernel_deriv, kernel_grad); // CHEKC THIS

            // CALCULATE PAIRWISE COEFFICIENT WITH NO GRADIENT
            float p_idx = sim_params->pressures[particle_idx];
            float p_i = sim_params->pressures[i];
            float density_idx = sim_params->densities[particle_idx];
            float density_i = sim_params->densities[i];

            float pairwise_coeff = glm_pow2(sim_params->particle_mass) * (p_idx / (glm_pow2(density_idx)) + p_i / (glm_pow2(density_i)));  

            // CALCULATE PAIRWISE FORCE
            glm_vec3_scale(kernel_grad, pairwise_coeff, pairwise_force);

            // ADD TO res_force 
            glm_vec3_add(res_force, pairwise_force, res_force);
        
        }
    } 

    glm_vec3_copy(res_force, dest_force);

}

void one_sim_step(fluid_sim_parameters* sim_params){

    // NO SPACE PARTITIONING SIM
    if (sim_params->is_running == 1){

        // CALCULATE DENSITIES
        update_sim_densities(sim_params);
    
        for (int i = 0; i < sim_params->n_particles; i += 1){

            vec3 total_force;
            vec3 accel;
            glm_vec3_zero(total_force);
            glm_vec3_zero(accel);

            // APPLY GRAVITY FORCE
            vec3 grav_force;
            glm_vec3_scale(y_axis, -sim_params->grav_scale * sim_params->particle_mass, grav_force);
            glm_vec3_add(total_force, grav_force, total_force);

            // APPLY OUT OF BOUNDS FORCE 
            vec3 out_of_bounds_force;
            get_out_of_bounds_force(sim_params, i, out_of_bounds_force);
            glm_vec3_add(total_force, out_of_bounds_force, total_force);

            // CALCULATE PRESSURE AT particle_idx = i
            update_sim_pressure_at_particle(sim_params, i);

            // APPLY PRESSURE FORCE
            vec3 pressure_force;
            get_pressure_force(sim_params, i, pressure_force);
            glm_vec3_add(total_force, pressure_force, total_force);

            // CALCULATE NEW ACCEL
            glm_vec3_scale(total_force, 1.0 / sim_params->particle_mass, accel);

            // CALCULATE NEW VELOCITY
            glm_vec3_scale(accel, sim_params->delta_time, accel);
            glm_vec3_add(sim_params->velocities[i], accel, sim_params->velocities[i]);


            // CALCULATE NEW POSITIONS
            vec3 delta_vel;
            glm_vec3_scale(sim_params->velocities[i], sim_params->delta_time, delta_vel);
            glm_vec3_add(sim_params->positions[i], delta_vel, sim_params->positions[i]);


        }
    }
    //printf("velocities[0][1]: %f\n", velocities[0][1]);

}