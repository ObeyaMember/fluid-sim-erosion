#include "fluid_simulation.h"

static vec3 x_axis = {1,0,0};
static vec3 y_axis = {0,1,0};
static vec3 z_axis = {0,0,1};

static vec3 neg_x_axis = {-1,0,0};
static vec3 neg_y_axis = {0,-1,0};
static vec3 neg_z_axis = {0,0,-1};

//                                              HELPER FUNCTIONS
static float clamp_pairwise_dist(float dist){
    return glm_clamp(dist, 0.001, 100000000);
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

static void get_grid_cell_dims(fluid_sim_parameters* sim_params, vec3 dest){
    vec3 dims = {sim_params->bound_dims[0] / sim_params->n_grid_cells_x, sim_params->bound_dims[1] / sim_params->n_grid_cells_y, sim_params->bound_dims[2] / sim_params->n_grid_cells_z};
    glm_vec3_copy(dims, dest);
}

static int is_valid_grid_config(fluid_sim_parameters* sim_params){
    return sim_params->n_grid_cells_x > 0 &&
           sim_params->n_grid_cells_y > 0 &&
           sim_params->n_grid_cells_z > 0 &&
           sim_params->n_grid_cells_total > 0;
}

static int vec3_is_finite(vec3 v){
    return isfinite(v[0]) && isfinite(v[1]) && isfinite(v[2]);
}

static int clamp_cell_idx(fluid_sim_parameters* sim_params, int cell_idx, int fallback_cell_idx){
    if (fallback_cell_idx < 0){
        fallback_cell_idx = 0;
    }

    if (fallback_cell_idx >= sim_params->n_grid_cells_total){
        fallback_cell_idx = sim_params->n_grid_cells_total - 1;
    }

    if (cell_idx < 0 || cell_idx >= sim_params->n_grid_cells_total){
        return fallback_cell_idx;
    }

    return cell_idx;
}

static int pos_to_cell(fluid_sim_parameters* sim_params, vec3 pos){
    int n_grid_cells_x = sim_params->n_grid_cells_x;
    int n_grid_cells_y = sim_params->n_grid_cells_y;
    int n_grid_cells_z = sim_params->n_grid_cells_z;

    float grid_cell_length = sim_params->bound_dims[0] / (float) sim_params->n_grid_cells_x;
    float grid_cell_height = sim_params->bound_dims[1] / (float) sim_params->n_grid_cells_y;
    float grid_cell_width = sim_params->bound_dims[2] / (float) sim_params->n_grid_cells_z;
    /* printf("grid_cell_length: %f\n", grid_cell_length);
    printf("grid_cell_height: %f\n", grid_cell_height);
    printf("grid_cell_width: %f\n", grid_cell_width); */

    vec3 p_pos;
    glm_vec3_copy(pos, p_pos);

    vec3 bound_pos;
    glm_vec3_copy(sim_params->bound_pos, bound_pos);
    
    vec3 bound_dims;
    glm_vec3_copy(sim_params->bound_dims, bound_dims);

    vec3 bound_dims_halved;
    glm_vec3_scale(bound_dims, 0.5, bound_dims_halved);

    vec3 first_corner_pos;
    glm_vec3_sub(bound_pos, bound_dims_halved, first_corner_pos);

    int cell_x = (int)floorf(((p_pos[0] - (first_corner_pos[0])) / grid_cell_length));
    int cell_y = (int)floorf(((p_pos[1] - (first_corner_pos[1])) / grid_cell_height));
    int cell_z = (int)floorf(((p_pos[2] - (first_corner_pos[2])) / grid_cell_width));

    // ----- x
    float dist_to_right_bound = p_pos[0] - (bound_pos[0] + bound_dims[0]/2.0);
    float dist_to_left_bound = (bound_pos[0] - bound_dims[0]/2.0) - p_pos[0];

    if (dist_to_right_bound >= 0){
        cell_x = n_grid_cells_x-1;
    }
    if (dist_to_left_bound >= 0){
        cell_x = 0;
    }

    // ----- y
    float dist_to_up_bound = p_pos[1] - (bound_pos[1] + bound_dims[1]/2.0);
    float dist_to_down_bound = (bound_pos[1] - bound_dims[1]/2.0) - p_pos[1];
    
    if (dist_to_up_bound >= 0){
        cell_y = n_grid_cells_y-1;
    }
    if (dist_to_down_bound >= 0){
        cell_y = 0;
    }

    // ----- z
    float dist_to_back_bound = p_pos[2] - (bound_pos[2] + bound_dims[2]/2.0);
    float dist_to_front_bound = (bound_pos[2] - bound_dims[2]/2.0) - p_pos[2];

    if (dist_to_back_bound >= 0){
        cell_z = n_grid_cells_z-1;
    }
    if (dist_to_front_bound >= 0){
        cell_z = 0;
    }

    // ----- space hashing formula
    int cell_idx = cell_z*n_grid_cells_y*n_grid_cells_x + cell_y*n_grid_cells_x + cell_x;
    return cell_idx;
}

static int contains_cell(int* cells, int count, int cell) {
    for (int i = 0; i < count; i++) {
        if (cells[i] == cell) return 1;
    }
    return 0;
}

static void get_needed_cells_from_pos(fluid_sim_parameters* sim_params, vec3 pos, int* needed_cells){
    //print_int_array(sim_params->grid_cells_num_partciles_count, sim_params->n_grid_cells_total);
    
    vec3 cell_dims;
    get_grid_cell_dims(sim_params, cell_dims);

    //printf("one go ----------------------------------\n");
    int local_cell_idx = 0;
    for (int x = 0; x <= 2; x += 1){
        for (int y = 0; y <= 2; y += 1){
            for (int z = 0; z <= 2; z += 1){
                vec3 offset = {-1 + x, -1 + y, -1 + z};
                
                offset[0] *= cell_dims[0];
                offset[1] *= cell_dims[1];
                offset[2] *= cell_dims[2];
                //print_vec3(offset);
                
                
                vec3 rel_pos;
                glm_vec3_add(pos, offset, rel_pos);
                
                
                int current_rel_cell = pos_to_cell(sim_params, rel_pos);

                //if (current_rel_cell < 0 || current_rel_cell >= sim_params->n_grid_cells_total) {
                //    needed_cells[local_cell_idx] = -1;
                //} else if (sim_params->grid_cells_num_partciles_count[current_rel_cell] == 0) {
                //    needed_cells[local_cell_idx] = -1;
                //} else {
                //    needed_cells[local_cell_idx] = current_rel_cell;
                //}

                if (current_rel_cell < 0 || current_rel_cell >= sim_params->n_grid_cells_total) {
                    needed_cells[local_cell_idx] = -1;
                } else if (contains_cell(needed_cells, local_cell_idx, current_rel_cell)) {
                    needed_cells[local_cell_idx] = -1;
                } else if (sim_params->grid_cells_num_partciles_count[current_rel_cell] == 0) {
                    needed_cells[local_cell_idx] = -1;
                } else {
                    needed_cells[local_cell_idx] = current_rel_cell;
                }
                
                local_cell_idx += 1;
            }
        }
    }
}

//                                              SMOOTHING KERNELS
static float smoothing_kernel_linear(fluid_sim_parameters* sim_params, float dist_to_center){
    return glm_clamp(1.0 - (1.0 / sim_params->particle_radius) * fabs(dist_to_center), 0.0, 1.0);
}

static float smoothing_kernel_linear_deriv(fluid_sim_parameters* sim_params, float dist_to_center){
    
    if (dist_to_center <= sim_params->particle_radius){
        return -(1.0 / sim_params->particle_radius);
    }
    else {
        return 0;
    }
}

static float smoothing_kernel_cubic(fluid_sim_parameters* sim_params, float dist_to_center){
    float r = sim_params->particle_radius;
    float vol = 3.14 * powf(r, 6.0) / 15.0;
    return glm_max(0, powf(r - dist_to_center, 3.0) / vol);
}

static float smoothing_kernel_cubic_deriv(fluid_sim_parameters* sim_params, float dist_to_center){
    float r = sim_params->particle_radius;
    if (dist_to_center <= sim_params->particle_radius){
        return -3 * powf(r-dist_to_center, 2.0) / powf(r, 3.0);
    }
    else {
        return 0;
    }
}

//                                         SIM STATE CALCULATIONS FUNCTIONS
static void update_sim_densities(fluid_sim_parameters* sim_params){

    // RESET DENSITY FOR LAST PARTICLE
    sim_params->densities[sim_params->n_particles-1] = sim_params->particle_mass * smoothing_kernel_cubic(sim_params, 0);

    for (int particle_idx = 0; particle_idx < sim_params->n_particles; particle_idx += 1){
        
        // RESET ALL DENSITIES EXCEPT FOR THE LAST ONE
        sim_params->densities[particle_idx] = sim_params->particle_mass * smoothing_kernel_cubic(sim_params, 0);
        
        for (int i = 0; i < sim_params->n_particles; i += 1){
            if (i != particle_idx){
                float pairwise_dist = glm_vec3_distance(sim_params->positions[particle_idx], sim_params->positions[i]);
                pairwise_dist = clamp_pairwise_dist(pairwise_dist);
                
                float pairwise_density = sim_params->particle_mass * smoothing_kernel_cubic(sim_params, pairwise_dist);
                sim_params->densities[particle_idx] += pairwise_density;
                //sim_params->densities[i] += pairwise_density;
            }     
        }
    }

    //sim_params->densities[0] = 0.2;
    //sim_params->densities[1] = 0.2;
    
    //printf("density[0]: %f\n", sim_params->densities[0]);
    //printf("density[1]: %f\n", sim_params->densities[1]);
    //printf("density[556]: %f\n", sim_params->densities[556]);   
}

static float get_density_at_particle_at_cell(fluid_sim_parameters* sim_params, int particle_idx, int cell_idx){
    float res_density = 0.0;
    int start_grid_idx = 0;
    if (cell_idx == 0){
        start_grid_idx = 0;
    }else {
        start_grid_idx = sim_params->grid_cells_num_particles_prefix_sums[cell_idx-1];
    }
    int n_partcile_count_at_cell_idx = sim_params->grid_cells_num_partciles_count[cell_idx];
    // print test
    //if (n_partcile_count_at_cell_idx >= 600 || n_partcile_count_at_cell_idx < 0){
    //        printf("n_partcile_count_at_cell_idx: %d\n", n_partcile_count_at_cell_idx);
    //    }

    for (int i = 0; i < sim_params->grid_cells_num_partciles_count[cell_idx]; i += 1){
        
        int p_idx = sim_params->grid[start_grid_idx + i];
        // print test
        //if (particle_idx >= 600 || particle_idx < 0 || p_idx >= 600 || p_idx < 0){
        //    printf("particle_idx: %d - p_idx: %d\n", particle_idx, p_idx);
        //}
        
        float pairwise_dist = glm_vec3_distance(sim_params->positions[particle_idx], sim_params->positions[p_idx]);
        pairwise_dist = clamp_pairwise_dist(pairwise_dist);
        res_density += sim_params->particle_mass * smoothing_kernel_cubic(sim_params, pairwise_dist);
        
    }
    

    return res_density;
}

static void update_sim_densities_at_particle_partitioned(fluid_sim_parameters* sim_params, int particle_idx, int* needed_cells){
    sim_params->densities[particle_idx] = 0.0;
     

    for (int i = 0; i < 27; i += 1){
        //int needed_cells_at_i = needed_cells[i];
        if (needed_cells[i] != -1){
            sim_params->densities[particle_idx] += get_density_at_particle_at_cell(sim_params, particle_idx, needed_cells[i]);
        }
    }
    

}

static void update_sim_pressure_at_particle(fluid_sim_parameters* sim_params, int particle_idx){
    
    float k = sim_params->stiffness_k;
    float gamma = sim_params->stiffness_gamma;
    float ref_density = sim_params->reference_density;
    float idx_density = sim_params->densities[particle_idx];
    
    sim_params->pressures[particle_idx] = (k / gamma) * ref_density * (powf(idx_density / ref_density, gamma) - 1.0);
    //printf("density[%d]: %f\n", particle_idx, sim_params->densities[particle_idx]);
    //printf("pressure[%d]: %f\n", particle_idx, sim_params->pressures[particle_idx]);
}

static void update_sim_pressure_at_particle_simple(fluid_sim_parameters* sim_params, int particle_idx){
    float k = sim_params->stiffness_k;
    float ref_density = sim_params->reference_density;
    float idx_density = sim_params->densities[particle_idx];

    float density_error = idx_density - ref_density;
    float pressure = density_error * k;
    sim_params->pressures[particle_idx] = pressure;

    //printf("density[%d]: %f\n", particle_idx, sim_params->densities[particle_idx]);
    //printf("pressure[%d]: %f\n", particle_idx, sim_params->pressures[particle_idx]);
}

static void update_sim_grid(fluid_sim_parameters* sim_params){
    if (!is_valid_grid_config(sim_params) ||
        sim_params->positions == NULL ||
        sim_params->grid_particle_cells == NULL ||
        sim_params->grid_cells_num_partciles_count == NULL ||
        sim_params->grid_cells_num_particles_prefix_sums == NULL ||
        sim_params->grid == NULL) {
        return;
    }

    int fallback_cell_idx = clamp_cell_idx(sim_params, pos_to_cell(sim_params, sim_params->bound_pos), 0);
    
    // UPDATE CELL IDX FOR EACH PARTICLE
    for (int i = 0; i < sim_params->n_particles; i += 1){
        int cell_idx = fallback_cell_idx;

        if (vec3_is_finite(sim_params->positions[i])) {
            cell_idx = pos_to_cell(sim_params, sim_params->positions[i]);
        }

        sim_params->grid_particle_cells[i] = clamp_cell_idx(sim_params, cell_idx, fallback_cell_idx);
        
        //if (v < 0 || v > sim_params->n_particles){
        //    printf("%d \n", v);
        //}
    }
    
    // RESET GRID
    for (int i = 0; i < sim_params->n_grid_cells_total; i += 1){
        sim_params->grid_cells_num_partciles_count[i] = 0;
        sim_params->grid_cells_num_particles_prefix_sums[i] = 0;
    }

    // UPDATE PREFIX SUMS
    // first just count cell occurences
    for (int i = 0; i < sim_params->n_particles; i += 1){
        
        //printf("grid_particle_cells[%d]: %d\n", i, sim_params->grid_particle_cells[i]);
        /* int needed_cells[27] = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1};
        get_needed_cells_from_pos(sim_params, sim_params->positions[i], needed_cells);
        //print_int_array(needed_cells, 27);
        for (int j = 0; j < 27; j += 1){
            if (needed_cells[j] != -1){
                sim_params->grid_cells_num_partciles_count[needed_cells[j]] += 1;
            }
            
        } */
        int p_cells_at_i = sim_params->grid_particle_cells[i];
        sim_params->grid_cells_num_partciles_count[p_cells_at_i] += 1;
        sim_params->grid_cells_num_particles_prefix_sums[sim_params->grid_particle_cells[i]] += 1;
        
    }
    //printf("count: \n");
    //print_int_array(sim_params->grid_cells_num_partciles_count, sim_params->n_grid_cells_total);
    // then compute prefix sums
    for (int i = 1; i < sim_params->n_grid_cells_total; i += 1){
        sim_params->grid_cells_num_particles_prefix_sums[i] += sim_params->grid_cells_num_particles_prefix_sums[i-1];
    }
    

    // ORDER PARTICLES BY THEIR CELL INDICES
    radix_sort_map(sim_params->grid, sim_params->grid_particle_cells, sim_params->n_particles);
    
}

static void update_grid_ground(fluid_sim_parameters* sim_params){
    // UPDATE CELL IDX FOR EACH PARTICLE
    terrain* t = sim_params->ground_terrain;
    heightmap* h_map = t->h_map;
    mesh* m = sim_params->ground_mesh;
    
    int res_x = h_map->map_res_x;
    int res_y = h_map->map_res_y;

    for (int i = 0; i < res_y; i += 1){ // this is the good one
            for (int j = 0; j < res_x; j += 1){ // this also the good
            int vertex_idx = (i*res_x + j)*3;
            vec3 pos = {0, 0, 0};
            
            pos[0] = m->pos[0] + m->mesh_vertices[vertex_idx + 0];
            pos[1] = m->pos[1] + m->mesh_vertices[vertex_idx + 1];
            pos[2] = m->pos[2] + m->mesh_vertices[vertex_idx + 2];
                
            
            
            


            int cell = pos_to_cell(sim_params, pos);
            sim_params->grid_hmap_points_cells[vertex_idx / 3] = cell;
        
        }
    }
    
    // RESET GRID
    for (int i = 0; i < sim_params->n_grid_cells_total; i += 1){
        sim_params->grid_cells_num_hmap_points_count[i] = 0;
        sim_params->grid_cells_num_hmap_points_prefix_sums[i] = 0;
    }

    // UPDATE PREFIX SUMS
    // first just count cell occurences
    for (int i = 0; i < res_y; i += 1){ // this is the good one
        for (int j = 0; j < res_x; j += 1){
            int vertex_idx = (i*res_x + j)*3;
            
            int p_cells_at_idx = sim_params->grid_hmap_points_cells[vertex_idx / 3];
            sim_params->grid_cells_num_hmap_points_count[p_cells_at_idx] += 1;
            sim_params->grid_cells_num_hmap_points_prefix_sums[sim_params->grid_hmap_points_cells[vertex_idx / 3]] += 1;
        }
    }
    
    // then compute prefix sums
    for (int c = 1; c < sim_params->n_grid_cells_total; c++) {
    sim_params->grid_cells_num_hmap_points_prefix_sums[c] +=
        sim_params->grid_cells_num_hmap_points_prefix_sums[c - 1];
}

    // ORDER PARTICLES BY THEIR CELL INDICES
    radix_sort_map(sim_params->grid_ground, sim_params->grid_hmap_points_cells, res_x * res_y);
}

//                                          FORCE CALCULATIONS FUNCTIONS

// air drag
static void get_air_drag_force(fluid_sim_parameters* sim_params, int particle_idx, vec3 dest_force){
    vec3 res_force;
    
    vec3 p_vel;
    glm_vec3_copy(sim_params->velocities[particle_idx], p_vel);
    
    glm_vec3_scale(p_vel, -(sim_params->air_drag) * sim_params->particle_mass * (1 / sim_params->delta_time), res_force);

    glm_vec3_copy(res_force, dest_force);
}

// out of bounds forces
static void get_bounce_damp_force(fluid_sim_parameters* sim_params, int particle_idx, vec3 wall_normal, vec3 dest_force){
    vec3 res_force;
    
    vec3 p_vel;
    glm_vec3_copy(sim_params->velocities[particle_idx], p_vel);
    
    vec3 p_vel_normal_comp = {p_vel[0] * abs(wall_normal[0]), p_vel[1] * abs(wall_normal[1]), p_vel[2] * abs(wall_normal[2])};

    glm_vec3_scale(p_vel_normal_comp, -(sim_params->out_of_bounds_bounce_damp) * sim_params->particle_mass * (1 / sim_params->delta_time), res_force);

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
        get_bounce_damp_force(sim_params, particle_idx, neg_x_axis ,bound_bounce_fric_force);
        glm_vec3_add(res_force, bound_bounce_fric_force, res_force);
    }
    if (dist_to_left_bound > 0){
        
        // BOUNCE OFF WALL
        vec3 bound_bounce_force = {sim_params->out_of_bounds_stiffness * dist_to_left_bound, 0, 0};
        glm_vec3_add(res_force, bound_bounce_force, res_force);

        // DAMPENING
        vec3 bound_bounce_fric_force;
        get_bounce_damp_force(sim_params, particle_idx, x_axis, bound_bounce_fric_force);
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
        get_bounce_damp_force(sim_params, particle_idx, neg_y_axis, bound_bounce_fric_force);
        glm_vec3_add(res_force, bound_bounce_fric_force, res_force);
    }
    if (dist_to_down_bound > 0){
        
        // BOUNCE OFF WALL
        vec3 bound_bounce_force = {0, sim_params->out_of_bounds_stiffness * dist_to_down_bound, 0};
        glm_vec3_add(res_force, bound_bounce_force, res_force);

        // DAMPENING
        vec3 bound_bounce_fric_force;
        get_bounce_damp_force(sim_params, particle_idx, y_axis, bound_bounce_fric_force);
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
        get_bounce_damp_force(sim_params, particle_idx, neg_z_axis, bound_bounce_fric_force);
        glm_vec3_add(res_force, bound_bounce_fric_force, res_force);
    }
    if (dist_to_front_bound > 0){
        
        // BOUNCE OFF WALL
        vec3 bound_bounce_force = {0, 0, sim_params->out_of_bounds_stiffness * dist_to_front_bound};
        glm_vec3_add(res_force, bound_bounce_force, res_force);

        // DAMPENING
        vec3 bound_bounce_fric_force;
        get_bounce_damp_force(sim_params, particle_idx, z_axis, bound_bounce_fric_force);
        glm_vec3_add(res_force, bound_bounce_fric_force, res_force);

        
    }

    // APPLY TOTAL BOUNCE + DAMP FORCE

    glm_vec3_copy(res_force, dest_force);

}

static void get_terrain_collision_force(fluid_sim_parameters* sim_params, int particle_idx, vec3 dest_force){
    vec3 res = {0, 0, 0};

    if (sim_params->ground_terrain == NULL ||
        sim_params->ground_mesh == NULL ||
        sim_params->grid_cells_num_hmap_points_count == NULL ||
        sim_params->grid_cells_num_hmap_points_prefix_sums == NULL ||
        sim_params->grid_ground == NULL) {
        glm_vec3_copy(res, dest_force);
        return;
    }

    mesh* m = sim_params->ground_mesh;
    int cell_idx = clamp_cell_idx(sim_params, pos_to_cell(sim_params, sim_params->positions[particle_idx]), 0);
    vec3 particle_pos;
    glm_vec3_copy(sim_params->positions[particle_idx], particle_pos);

    int start_grid_idx = 0;
    if (cell_idx != 0){
        start_grid_idx = sim_params->grid_cells_num_hmap_points_prefix_sums[cell_idx - 1];
    }

    int min_dist_idx = -1;
    float min_dist = 0.0f;
    int n_hmap_points_count_at_cell_idx = sim_params->grid_cells_num_hmap_points_count[cell_idx];
    vec3 min_point_pos = {0, 0, 0};
    for (int i = 0; i < n_hmap_points_count_at_cell_idx; i += 1){
        int point_idx = sim_params->grid_ground[start_grid_idx + i];
        int vertex_idx = point_idx * 3;
        vec3 new_point_pos = {
            m->pos[0] + m->mesh_vertices[vertex_idx + 0],
            m->pos[1] + m->mesh_vertices[vertex_idx + 1],
            m->pos[2] + m->mesh_vertices[vertex_idx + 2],
        };
        float dist = glm_vec3_distance(new_point_pos, particle_pos);

        if (min_dist_idx == -1 || dist < min_dist){
            min_dist = dist;
            min_dist_idx = point_idx;
            glm_vec3_copy(new_point_pos, min_point_pos);
        }
    }

    if (min_dist_idx == -1){
        glm_vec3_copy(res, dest_force);
        return;
    }
    
    float h_diff = particle_pos[1] - (min_point_pos[1]);
    //printf("min_dist_idx: %d\n", min_dist_idx);
    //printf("val_left: %f\n", particle_pos[1]);
    //printf("val_right: %f\n", m->pos[1]);
    //printf("h_diff: %f\n", h_diff);
    if (h_diff < 0){
        float k = sim_params->out_of_bounds_stiffness;
        res[1] = -k * (h_diff);
        //res[1] = -10.0;
        //printf("aaaa\n");    
    }
    
    // Terrain response is still under construction; keep the force neutral,
    // but avoid reading invalid mesh data while looking up nearest points.
    glm_vec3_copy(res, dest_force);
}

// pressure
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

            float pairwise_coeff = -glm_pow2(sim_params->particle_mass) * (p_idx / (glm_pow2(density_idx)) + p_i / (glm_pow2(density_i)));  

            // CALCULATE PAIRWISE FORCE
            glm_vec3_scale(kernel_grad, pairwise_coeff, pairwise_force);

            // ADD TO res_force 
            glm_vec3_add(res_force, pairwise_force, res_force);
        
        }
    } 

    glm_vec3_copy(res_force, dest_force);

}

static void get_pressure_force_simple(fluid_sim_parameters* sim_params, int particle_idx, vec3 dest_force){
    
    vec3 res_force = {0,0,0};
    for (int i = 0; i < sim_params->n_particles; i += 1){
        if (i != particle_idx){
            
            vec3 pairwise_force;
            
            // CALCULATE KERNEL GRADIENT
            vec3 kernel_grad;
            vec3 dir_idx_to_i;
            float pairwise_dist = glm_vec3_distance(sim_params->positions[particle_idx], sim_params->positions[i]);
            pairwise_dist = clamp_pairwise_dist(pairwise_dist);

            float kernel_deriv = smoothing_kernel_cubic_deriv(sim_params, pairwise_dist);
        
            glm_vec3_sub(sim_params->positions[particle_idx], sim_params->positions[i], dir_idx_to_i);
            glm_vec3_normalize(dir_idx_to_i);
            glm_vec3_scale(dir_idx_to_i, kernel_deriv, kernel_grad); // CHEKC THIS

            // CALCULATE PAIRWISE COEFFICIENT WITH NO GRADIENT
            float p_idx = sim_params->pressures[particle_idx];
            float p_i = sim_params->pressures[i];
            float density_idx = sim_params->densities[particle_idx];
            float density_i = sim_params->densities[i];

            float pairwise_coeff = -p_i * sim_params->particle_mass / density_i;

            // CALCULATE PAIRWISE FORCE
            glm_vec3_scale(kernel_grad, pairwise_coeff, pairwise_force);

            // ADD TO res_force 
            glm_vec3_add(res_force, pairwise_force, res_force);
        
        }
    } 

    glm_vec3_copy(res_force, dest_force);

}

static void get_pressure_force_simple_at_particle_at_cell(fluid_sim_parameters* sim_params, int particle_idx, int cell_idx, vec3 dest_force){
    vec3 res_pressure_force;
    glm_vec3_zero(res_pressure_force);
    int start_grid_idx = 0;
    if (cell_idx == 0){
        start_grid_idx = 0;
    }else {
        //printf("cell idx: %d\n", cell_idx);
        start_grid_idx = sim_params->grid_cells_num_particles_prefix_sums[cell_idx-1];
        
    }

    for (int i = 0; i < sim_params->grid_cells_num_partciles_count[cell_idx]; i += 1){
            
        int p_idx = sim_params->grid[start_grid_idx + i];
        if (p_idx == particle_idx) continue;
        

        vec3 pairwise_force;
            
        // CALCULATE KERNEL GRADIENT
        vec3 kernel_grad;
        vec3 dir_idx_to_i;
        float pairwise_dist = glm_vec3_distance(sim_params->positions[particle_idx], sim_params->positions[p_idx]);
        pairwise_dist = clamp_pairwise_dist(pairwise_dist);

        float kernel_deriv = smoothing_kernel_cubic_deriv(sim_params, pairwise_dist);
        
        glm_vec3_sub(sim_params->positions[particle_idx], sim_params->positions[p_idx], dir_idx_to_i);
        glm_vec3_normalize(dir_idx_to_i);
        glm_vec3_scale(dir_idx_to_i, kernel_deriv, kernel_grad); // CHEKC THIS
            
        // CALCULATE PAIRWISE COEFFICIENT WITH NO GRADIENT
        float press_idx = sim_params->pressures[particle_idx];
        float press_i = sim_params->pressures[p_idx];
        float density_idx = sim_params->densities[particle_idx];
        float density_i = sim_params->densities[p_idx];

        //float pairwise_coeff = -press_i * sim_params->particle_mass / density_i;
        float pairwise_coeff =
            -sim_params->particle_mass *
            (press_idx / (density_idx * density_idx)
             + press_i / (density_i * density_i));

        // CALCULATE PAIRWISE FORCE
        glm_vec3_scale(kernel_grad, pairwise_coeff, pairwise_force);
        //printf("density_idx[%d]: %f\n", particle_idx, density_idx);
        //printf("density_i[%d]: %f\n", particle_idx, density_i);
        //printf("paiwise_coeff[%d, %d]: %f\n", particle_idx, p_idx, pairwise_coeff);
        //printf("pairwise_force[%d, %d]: ", particle_idx, p_idx);
        //print_vec3(pairwise_force);

        // ADD TO res_pressure_force 
        glm_vec3_add(res_pressure_force, pairwise_force, res_pressure_force);
            
    }

    glm_vec3_copy(res_pressure_force, dest_force);

}

static void get_pressure_force_simple_at_particle_partitioned(fluid_sim_parameters* sim_params, int particle_idx, int* needed_cells, vec3 dest_force){
    vec3 res_pressure_force;
    glm_vec3_zero(res_pressure_force);

    for (int i = 0; i < 27; i += 1){
        vec3 pressure_at_cell;
        glm_vec3_zero(pressure_at_cell);
        if (needed_cells[i] != -1){ 
            get_pressure_force_simple_at_particle_at_cell(sim_params, particle_idx, needed_cells[i], pressure_at_cell);
            glm_vec3_add(pressure_at_cell, res_pressure_force, res_pressure_force);
        }
        
        
        
       
    }

    glm_vec3_copy(res_pressure_force, dest_force);
}

//                                               SETUP FUNCTIONS
static void setup_arrays(fluid_sim_parameters* sim_params){
    // SIM STATE
    sim_params->positions = malloc(sim_params->n_particles * sizeof(vec3));
    sim_params->velocities = malloc(sim_params->n_particles * sizeof(vec3));
    sim_params->densities = malloc(sim_params->n_particles * sizeof(float));
    sim_params->pressures = malloc(sim_params->n_particles * sizeof(float));

    // SIM GRID
    sim_params->grid_particle_cells = malloc(sim_params->n_particles * sizeof(int));
    sim_params->grid_cells_num_partciles_count = malloc(sim_params->n_grid_cells_total * sizeof(int));
    sim_params->grid_cells_num_particles_prefix_sums = malloc(sim_params->n_grid_cells_total * sizeof(int));
    sim_params->grid = malloc(sim_params->n_particles * sizeof(int));

    // GROUND GRID
    terrain* t = sim_params->ground_terrain;
    heightmap* h_map = t->h_map;
    int n_hmap_points = h_map->map_res_x * h_map->map_res_y;
    sim_params->n_hmap_points = n_hmap_points;
    sim_params->grid_hmap_points_cells = malloc(n_hmap_points * sizeof(int));
    sim_params->grid_cells_num_hmap_points_count = malloc(sim_params->n_grid_cells_total * sizeof(int));
    sim_params->grid_cells_num_hmap_points_prefix_sums = malloc(sim_params->n_grid_cells_total * sizeof(int));
    sim_params->grid_ground = malloc(n_hmap_points * sizeof(int));
    //print_terrain_data(t);

}

static void setup_particle_positions_in_box_uniform(fluid_sim_parameters* sim_params){
    vec3 bound_dims;
    vec3 bound_dims_halved;
    vec3 bound_pos;
    glm_vec3_copy(sim_params->bound_dims, bound_dims);
    glm_vec3_copy(sim_params->bound_pos, bound_pos);
    glm_vec3_scale(bound_dims, 0.5, bound_dims_halved);

    vec3 start_pos;
    glm_vec3_sub(bound_pos, bound_dims_halved, start_pos);
    
    int l_x = sim_params->spawn_box_dims[0] / (float)sim_params->n_particles_x;
    int l_y = sim_params->spawn_box_dims[1] / (float)sim_params->n_particles_y;
    int l_z = sim_params->spawn_box_dims[2] / (float)sim_params->n_particles_z;

    vec3 current_pos;
    glm_vec3_copy(start_pos, current_pos);
    
    int idx = 0;
    for (int x = 0; x < sim_params->n_particles_x; x += 1){
        for (int y = 0; y < sim_params->n_particles_y; y += 1){
            for (int z = 0; z < sim_params->n_particles_z; z += 1){
                vec3 to_add = {x * l_x, y * l_y, z * l_z};
                glm_vec3_copy(current_pos, sim_params->positions[idx]);
                glm_vec3_add(current_pos, to_add, current_pos);
                idx += 1;
            }
        }
    }

    
}

static void setup_particle_positions_in_box(fluid_sim_parameters* sim_params){
    printf("sim_id: %d\n", sim_params->sim_id);
    printf("n_parrticles: %d\n", sim_params->n_particles);
    printf("mode: %d\n", sim_params->positions_setup_mode);
    
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

    //setup_particle_positions_in_box_test(sim_params);
    
    update_sim_densities(sim_params);
    
    

}

static void setup_particle_positions_from_other_sim(fluid_sim_parameters* sim_params){
    for (int i = 0; i < sim_params->n_particles; i += 1){
        //printf("aa ");
        vec3 pos_idx;
        //printf("copy_n_particles: %d", (sim_params->sim_to_copy)->n_particles);
        glm_vec3_sub((sim_params->sim_to_copy)->positions[i], (sim_params->sim_to_copy)->spawn_box_pos, pos_idx);
        glm_vec3_add(pos_idx, sim_params->spawn_box_pos, pos_idx);
        glm_vec3_copy(pos_idx, sim_params->positions[i]);
    }
}

static void setup_particle_velocities(fluid_sim_parameters* sim_params){
    for (int i = 0; i < sim_params->n_particles; i += 1){
        vec3 start_vel = {-10,-2,0};
        glm_vec3_copy(start_vel, sim_params->velocities[i]);
        
    }
}

static void setup_particle_densities(fluid_sim_parameters* sim_params){
    for (int i = 0; i < sim_params->n_particles; i += 1){
        sim_params->densities[i] = sim_params->reference_density;
    }
}

static void setup_particle_pressures(fluid_sim_parameters* sim_params){
    for (int i = 0; i < sim_params->n_particles; i += 1){
        sim_params->pressures[i] = 0;
    }
}

// assumes postions have already been set up
static void setup_sim_grid(fluid_sim_parameters* sim_params){
    int n_cells_x = sim_params->n_grid_cells_x;
    int n_cells_y = sim_params->n_grid_cells_y;
    int n_cells_z = sim_params->n_grid_cells_z;
    int n_cells_total = n_cells_x*n_cells_y*n_cells_z;
    sim_params->n_grid_cells_total = n_cells_total;
    printf("n_total_grid_cells: %d\n", sim_params->n_grid_cells_total);


    for (int i = 0; i < sim_params->n_particles; i += 1){
        sim_params->grid_particle_cells[i] = i;
        sim_params->grid[i] = i;
    }

    for (int i = 0; i < sim_params->n_grid_cells_total; i += 1){
        sim_params->grid_cells_num_partciles_count[i] = 0;
        sim_params->grid_cells_num_particles_prefix_sums[i] = 0;
    }

    for (int i = 0; i < sim_params->n_hmap_points; i += 1){
        sim_params->grid_hmap_points_cells[i] = 0;
        sim_params->grid_ground[i] = i;
    }

    for (int i = 0; i < sim_params->n_grid_cells_total; i += 1){
        sim_params->grid_cells_num_hmap_points_count[i] = 0;
        sim_params->grid_cells_num_hmap_points_prefix_sums[i] = 0;
    }

    update_sim_grid(sim_params);
    
    /* printf("grid:\n");
    print_int_array(sim_params->grid, sim_params->n_particles);
    printf("----------\n");

    printf("particle_cells:\n");
    print_int_array(sim_params->grid_particle_cells, sim_params->n_particles);
    printf("----------\n"); */

    /* printf("cells prefix sums:\n");
    print_int_array(sim_params->grid_cells_num_particles_prefix_sums, sim_params->n_grid_cells_total);
    printf("----------\n"); */

}

void fluid_sim_setup(fluid_sim_parameters* sim_params){
    
    // setup n_particles
    if (sim_params->positions_setup_mode == 1){
        sim_params->n_particles = sim_params->n_particles_x * sim_params->n_particles_y * sim_params->n_particles_z;
    }
    // setup n_grid_cells
    int n_cells_x = sim_params->n_grid_cells_x;
    int n_cells_y = sim_params->n_grid_cells_y;
    int n_cells_z = sim_params->n_grid_cells_z;
    int n_cells_total = n_cells_x*n_cells_y*n_cells_z;
    sim_params->n_grid_cells_total = n_cells_total;
    setup_arrays(sim_params);
    // setup
    
    if (sim_params->positions_setup_mode == 0){
        setup_particle_positions_in_box(sim_params);
    }else if (sim_params->positions_setup_mode == 1){
        setup_particle_positions_in_box_uniform(sim_params);
    }else if (sim_params->positions_setup_mode == 2){
        setup_particle_positions_from_other_sim(sim_params);
    }
    


    setup_particle_velocities(sim_params);
    setup_particle_densities(sim_params);
    setup_particle_pressures(sim_params);
    setup_sim_grid(sim_params);

}

// SIMULATION
void one_sim_step(fluid_sim_parameters* sim_params){

    // test smoothing kernel
    // printf("smoothing kernel test: %f\n", smoothing_kernel_linear(sim_params, 1));
    
    // NO SPACE PARTITIONING SIM
    if (sim_params->is_running == 1){

        // SPATIAL PARTITIONING GRID
        update_sim_grid(sim_params);
        // CALCULATE DENSITIES
        update_sim_densities(sim_params);

        // CALCULATE PRESSURES
        for (int i = 0; i < sim_params->n_particles; i += 1){
            // CALCULATE PRESSURE AT particle_idx = i
            update_sim_pressure_at_particle_simple(sim_params, i);
        }
    
        for (int i = 0; i < sim_params->n_particles; i += 1){

            vec3 total_force;
            vec3 accel;
            glm_vec3_zero(total_force);
            glm_vec3_zero(accel);

            // APPLY GRAVITY FORCE
            vec3 grav_force;
            glm_vec3_scale(y_axis, -sim_params->grav_scale * sim_params->particle_mass, grav_force);
            glm_vec3_add(total_force, grav_force, total_force);

            // APPLY AIR DRAG FORCE
            vec3 air_drag_force;
            get_air_drag_force(sim_params, i, air_drag_force);
            glm_vec3_add(total_force, air_drag_force, total_force);


            // APPLY OUT OF BOUNDS FORCE 
            vec3 out_of_bounds_force;
            get_out_of_bounds_force(sim_params, i, out_of_bounds_force);
            glm_vec3_add(total_force, out_of_bounds_force, total_force);

            // CALCULATE PRESSURE AT particle_idx = i
            //update_sim_pressure_at_particle(sim_params, i);

            // APPLY PRESSURE FORCE
            vec3 pressure_force;
            //get_pressure_force(sim_params, i, pressure_force);
            get_pressure_force_simple(sim_params, i, pressure_force);
            glm_vec3_add(total_force, pressure_force, total_force);
            //printf("Pressure force mag[%d]: %f\n", i, glm_vec3_norm(pressure_force));

            // CALCULATE NEW ACCEL
            glm_vec3_scale(total_force, 1.0 / sim_params->particle_mass, accel);

            // CALCULATE NEW VELOCITY
            glm_vec3_scale(accel, sim_params->delta_time, accel);
            glm_vec3_add(sim_params->velocities[i], accel, sim_params->velocities[i]);


            // CALCULATE NEW POSITIONS
            vec3 delta_vel;
            glm_vec3_scale(sim_params->velocities[i], sim_params->delta_time, delta_vel);
            if (sim_params->is_frozen == 0){
                glm_vec3_add(sim_params->positions[i], delta_vel, sim_params->positions[i]);
            }
            


        }
    }
    //printf("velocities[0][1]: %f\n", velocities[0][1]);

}

void one_sim_step_partitioned(fluid_sim_parameters* sim_params){
    if (sim_params->is_running == 1){
        update_sim_grid(sim_params);
        update_grid_ground(sim_params);

        // UPDATE ALL DENSITIES AND ALL PRESSURES
        for (int i = 0; i < sim_params->n_particles; i += 1){
            // 27 bcs thats all the cells around a certain cell (it included)
            int needed_cells[27] = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1};
            get_needed_cells_from_pos(sim_params, sim_params->positions[i], needed_cells);
            
            // UPDATE DENSITY AT PARTICLE i
            //printf("count: \n");
            //print_int_array(sim_params->grid_cells_num_partciles_count, sim_params->n_grid_cells_total);
            update_sim_densities_at_particle_partitioned(sim_params, i, &needed_cells[0]);
            
            // UPDATE PRESSURE AT PARTICLE i
            update_sim_pressure_at_particle_simple(sim_params, i);   
        }

        // CALCULATE PER PARTICLE FORCES AND ACC. UPDATE VEL AND POS.
        for (int i = 0; i < sim_params->n_particles; i += 1){
            int needed_cells[27] = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1};
            get_needed_cells_from_pos(sim_params, sim_params->positions[i], needed_cells);
            
            // ------------------ forces -----------------
            vec3 total_force;
            vec3 accel;
            glm_vec3_zero(total_force);
            glm_vec3_zero(accel);

            // CALCULATE PRESSURE FORCE
            vec3 pressure_force;
            get_pressure_force_simple_at_particle_partitioned(sim_params, i, needed_cells, pressure_force);
            glm_vec3_add(pressure_force, total_force, total_force);            
            // APPLY GRAVITY FORCE
            vec3 grav_force;
            glm_vec3_scale(y_axis, -sim_params->grav_scale * sim_params->particle_mass, grav_force);
            glm_vec3_add(total_force, grav_force, total_force);

            // APPLY AIR DRAG FORCE
            vec3 air_drag_force;
            get_air_drag_force(sim_params, i, air_drag_force);
            glm_vec3_add(total_force, air_drag_force, total_force);


            // APPLY OUT OF BOUNDS FORCE 
            vec3 out_of_bounds_force;
            get_out_of_bounds_force(sim_params, i, out_of_bounds_force);
            glm_vec3_add(total_force, out_of_bounds_force, total_force);

            // APPLY TERRAIN COLLISION FORCE
            vec3 terrain_collision_force;
            get_terrain_collision_force(sim_params, i, terrain_collision_force);
            glm_vec3_add(total_force, terrain_collision_force, total_force);

            /* // CALCULATE PRESSURE AT particle_idx = i
            //update_sim_pressure_at_particle(sim_params, i);

            // APPLY PRESSURE FORCE
            vec3 pressure_force;
            //get_pressure_force(sim_params, i, pressure_force);
            get_pressure_force_simple(sim_params, i, pressure_force);
            glm_vec3_add(total_force, pressure_force, total_force);
            //printf("Pressure force mag[%d]: %f\n", i, glm_vec3_norm(pressure_force)); */

            // CALCULATE NEW ACCEL
            glm_vec3_scale(total_force, 1.0 / sim_params->particle_mass, accel);

            // CALCULATE NEW VELOCITY
            glm_vec3_scale(accel, sim_params->delta_time, accel);
            glm_vec3_add(sim_params->velocities[i], accel, sim_params->velocities[i]);


            // CALCULATE NEW POSITIONS
            vec3 delta_vel;
            glm_vec3_scale(sim_params->velocities[i], sim_params->delta_time, delta_vel);
            if(sim_params->is_frozen == 0){
                glm_vec3_add(sim_params->positions[i], delta_vel, sim_params->positions[i]);
            }
            

            
            
        }
        
    }
}

// END SIM
void fluid_sim_end(fluid_sim_parameters* sim_params){
    free(sim_params->grid_particle_cells);
    free(sim_params->grid_cells_num_partciles_count);
    free(sim_params->grid_cells_num_particles_prefix_sums);
   
    free(sim_params->grid);
    free(sim_params->positions);
    free(sim_params->velocities);
    free(sim_params->densities);
    free(sim_params->pressures);
}
