#include "fluid_test_scene.h"

//                                      SCENE VARIABLES
//                                          WORLD
vec3 x_axis = {1,0,0};
vec3 y_axis = {0,1,0};
vec3 z_axis = {0,0,1};

//                                           CAMERA  
camera_3d camera_1 = {
    .camera_fov = 100.0,
    .camera_aspect = 1.0,
    .camera_near = 0.01,
    .camera_far = 1000.0,
    .camera_pitch = 0.0,
    .camera_yaw = 0.0,
    .camera_move_speed = 10.0,
    .camera_look_speed = 10.0,
    .camera_zoom_speed = 1.0,
    .camera_pos = {0,0,-10},
    .camera_dir = {0,1,0},

    //.view,
    //.projection,
};

//                                           ENGINE 
float delta_time = 1.0;
float last_frame_time = 0.0;
float mouse_x = 400;
float mouse_y = 400;

//                                          DATA
// FLUID SIM 1
fluid_sim_parameters fluid_sim_params_1 = {
    
    .sim_id = 1,

    // BOUNDINGS
    .bound_pos = {0,0,0},
    .bound_dims = {20, 20, 20},
    .out_of_bounds_stiffness = 500,
    .out_of_bounds_bounce_damp = 0.05,
    
    // SPATIAL GRID
    .n_grid_cells_x = 5,
    .n_grid_cells_y = 5,
    .n_grid_cells_z = 2,
    .n_grid_cells_total = 1000, // doesn't need to be hand stated

    // SPAWN BOX
    .spawn_box_pos = {0, 0, 0},
    .spawn_box_dims = {11, 11, 11},//{40, 10, 5},

    .positions_setup_mode = 2,
    .is_frozen = 0,
    
    .n_particles_x = 1,
    .n_particles_y = 1,
    .n_particles_z = 10,
    .n_particles = 200,

    // PARTICLES PARAMETERS
    
    .grav_scale = 10.0,
    .particle_mass = 1.0,
    .particle_radius = 7.0,
    .stiffness_k = 30,
    .stiffness_gamma = 3.0,
    .reference_density = 0.05,
    .air_drag = 0.005
};
fluid_render_paramters fluid_render_params_1 = {
    .render_radius = 0.5
};

// FLUID SIM 2
fluid_sim_parameters fluid_sim_params_2 = {
    .sim_id = 2,

    // BOUNDINGD
    .bound_pos = {25,0,0},
    .bound_dims = {20, 20, 7},
    .out_of_bounds_stiffness = 500,
    .out_of_bounds_bounce_damp = 0.05,
    
    // SPATIAL GRID
    .n_grid_cells_x = 5,
    .n_grid_cells_y = 5,
    .n_grid_cells_z = 2,
    .n_grid_cells_total = 1000, // doesn't need to be hand stated

    // SPAWN BOX
    .spawn_box_pos = {25, 0, 0},
    .spawn_box_dims = {20, 20, 7},//{40, 10, 5},
    
    .positions_setup_mode = 0,
    .is_frozen = 0,
    
    .n_particles_x = 10,
    .n_particles_y = 10,
    .n_particles_z = 10,
    .n_particles = 300,
    
    // PARTICLES PARAMETERS

    .grav_scale = 10.0,
    .particle_mass = 1.0,
    .particle_radius = 4.0,
    .stiffness_k = 30,
    .stiffness_gamma = 3.0,
    .reference_density = 0.05,
    .air_drag = 0.005
};
fluid_render_paramters fluid_render_params_2 = {
    .render_radius = 0.5
};

// GROUND
// ground heightmap
heightmap ground_h_map = {
    .height_factor = 10.0,
};
// ground terrain
terrain ground_terrain;
// ground mesh
mesh ground_mesh;

mesh_renderer ground_m_renderer;
//                                          TOOLS FUNCTIONS
float dome(void* weights, int x, int z){
    vec2 w;
    glm_vec2_copy(weights, w);
    float res = 1.0 - powf(x/w[0], 2) - powf(z/w[1], 2);
    if (res > 1.0){
        return 1.0;
    }
    if (res < 0.0){
        return 0.0;
    }
    return res;
}

//                                        SCENE FUNCTIONS

//                                             SETUP


//                         -------------------      SCENE SETUP AND LOOP       --------------------
void fluid_test_scene_setup(GLFWwindow* window){
    // CAMERA
    camera_setup(&camera_1);
    //glEnable(GL_CULL_FACE);    !!!!!!!!!!!!!!!! face order baaad 
    

    //  FLUID SIM 2
    // fluid sim
    fluid_sim_setup(&fluid_sim_params_2);
    
    // fluid render
    fluid_renderer_setup(&fluid_render_params_2, &fluid_sim_params_2);
    
      //  FLUID SIM 1
    // fluid sim
    fluid_sim_params_1.sim_to_copy = &fluid_sim_params_2;
    //fluid_sim_setup(&fluid_sim_params_1);
    
    
    // fluid render
    //fluid_renderer_setup(&fluid_render_params_1, &fluid_sim_params_1);

    //              GROUND
    // ground heightmap
    heightmap_setup(&ground_h_map, 30, 30);
    vec3 dome_weights = {ground_h_map.map_res_x, ground_h_map.map_res_y};
    heightmap_from_function(dome_weights ,&ground_h_map, dome);
    //heightmap_white(&ground_h_map);
    //print_heightmap(&ground_h_map);

    // ground terrain
    vec3 t_pos = {0, 0, 0};
    vec3 t_dims = {10, 1, 10};
    //glm_vec3_scale(fluid_sim_params_2.bound_dims, .5, half_bound_dims);

    glm_vec3_copy(fluid_sim_params_2.bound_pos, t_pos);
    glm_vec3_copy(fluid_sim_params_2.bound_dims, t_dims);
    t_pos[1] -= 0.5 * fluid_sim_params_2.bound_dims[1];
    ground_terrain = new_terrain_from_heightmap(&ground_h_map, t_pos, t_dims);
    // ground mesh

    ground_mesh = new_mesh_from_terrain(&ground_terrain);
    mesh_renderer_setup(&ground_m_renderer, &ground_mesh);

    
    // PRINTS
    //printf("bound pos: (%f, %f, %f)\n", fluid_sim_params_2.bound_pos[0], fluid_sim_params_2.bound_pos[1], fluid_sim_params_2.bound_pos[2]);
    //printf("terrain pos: (%f, %f, %f)\n", ground_terrain.pos[0], ground_terrain.pos[1], ground_terrain.pos[2]);
    //
    //printf("resx, resy: %d, %d \n", ground_h_map.map_res_x, ground_h_map.map_res_y);
    //printf("mesh pos: (%f, %f, %f)\n", ground_mesh.pos[0], ground_mesh.pos[1], ground_mesh.pos[2]);
    //printf("mesh dims: (%f, %f, %f)\n", ground_mesh.dims[0], ground_mesh.dims[1], ground_mesh.dims[2]);
    //printf("mesh vertices: \n");
    //print_mesh_vertices(&ground_mesh);
    //printf("mesh indices: \n");
    //print_mesh_indices_normalized(&ground_mesh);

    // SOMETHING
    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    
}

void fluid_test_scene_main_loop(GLFWwindow* window){
    // DELTA TIME
    float new_frame_time = glfwGetTime();
    delta_time = new_frame_time - last_frame_time;
    last_frame_time = new_frame_time;

    // SOMETHING
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.3f, 0.4f, 0.6f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //  FLUID SIM 1
    // next sim step
    fluid_sim_params_1.delta_time = delta_time;
    pause_sim(window, &fluid_sim_params_1);
    //one_sim_step(&fluid_sim_params_1);
    //one_sim_step_partitioned(&fluid_sim_params_1);
//
    ////  fluid renderer
    //// draw bounding
    //fluid_renderer_loop_draw_bounding(&camera_1, &fluid_render_params_1, &fluid_sim_params_1);
//
    //// draw particles
    //fluid_renderer_loop_draw_fluid_particles(&camera_1, &fluid_render_params_1, &fluid_sim_params_1);
    //
    //// draw grid used cells
    //fluid_renderer_loop_draw_sim_grid(&camera_1, &fluid_render_params_1, &fluid_sim_params_1);

    //  FLUID SIM 2
    // next sim step
    fluid_sim_params_2.delta_time = delta_time;
    pause_sim(window, &fluid_sim_params_2);
    //one_sim_step(&fluid_sim_params_2);
    one_sim_step_partitioned(&fluid_sim_params_2);

    //  fluid renderer
    // draw bounding
    fluid_renderer_loop_draw_bounding(&camera_1, &fluid_render_params_2, &fluid_sim_params_2);

    // draw particles
    fluid_renderer_loop_draw_fluid_particles(&camera_1, &fluid_render_params_2, &fluid_sim_params_2);
    
    // draw grid used cells
    fluid_renderer_loop_draw_sim_grid(&camera_1, &fluid_render_params_2, &fluid_sim_params_2);

    //printf("n_total_grid_cells_x_during: %f\n", fluid_sim_params_1.n_grid_cells_x);

    // GROUND   
    mesh_renderer_loop_draw_mesh(&camera_1, &ground_m_renderer, &ground_mesh);
    //printf("mesh vertices: \n");
    //print_mesh_vertices(&ground_mesh);

    // ---------------------------tests------------------------------------------------
    
    float avg_desnsity_sim_1 = 0;
    float avg_desnsity_sim_2 = 0;
    //for (int i =0; i < fluid_sim_params_1.n_particles; i += 1){
    //    printf("sim_id: %d -> ", fluid_sim_params_1.sim_id);
    //    printf("densities[%d]: %f\n", i, fluid_sim_params_1.densities[i]);
    //    
    //    printf("sim_id: %d -> ", fluid_sim_params_2.sim_id);
    //    printf("densities[%d]: %f\n", i, fluid_sim_params_2.densities[i]);
    //    printf("-\n");
    //    
    //avg_desnsity_sim_1 += fluid_sim_params_1.densities[i];
        
    //
    //}

    for (int i =0; i < fluid_sim_params_2.n_particles; i += 1){
    //    printf("sim_id: %d -> ", fluid_sim_params_1.sim_id);
    //    printf("densities[%d]: %f\n", i, fluid_sim_params_1.densities[i]);
    //    
    //    printf("sim_id: %d -> ", fluid_sim_params_2.sim_id);
    //    printf("densities[%d]: %f\n", i, fluid_sim_params_2.densities[i]);
    //    printf("-\n");
    //    
        
        avg_desnsity_sim_2 += fluid_sim_params_2.densities[i];
    //
    }

    avg_desnsity_sim_1 /= (float)fluid_sim_params_1.n_particles;
    avg_desnsity_sim_2 /= (float)fluid_sim_params_2.n_particles;

    //printf("sim_id: %d -> ", fluid_sim_params_1.sim_id);
    //printf("avg_density: %f\n", avg_desnsity_sim_1);
//
    //printf("sim_id: %d -> ", fluid_sim_params_2.sim_id);
    //printf("avg_density: %f\n", avg_desnsity_sim_2);
    //printf("-------\n");

    // --------------------------------------------------------------------------------
    
    // CAMERA 1
    camera_loop(window, &camera_1, &mouse_x, &mouse_y, delta_time);
}   

void fluid_test_scene_end(GLFWwindow* window){
    // DO FREE FLUID SIM AND RENDER
    fluid_sim_end(&fluid_sim_params_1);
    fluid_sim_end(&fluid_sim_params_2);
    fluid_renderer_end(&fluid_render_params_1);
    fluid_renderer_end(&fluid_render_params_2);
    free_heightmap(&ground_h_map);
    //free_terrain(&ground_terrain); // also frees heightmap
    //free_mesh(&ground_mesh);
}