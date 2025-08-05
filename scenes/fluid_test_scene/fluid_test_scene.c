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
    .camera_pos = {0,0,-30},
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

fluid_sim_parameters fluid_sim_params = {
    .bound_pos = {0,0,0},
    .bound_dims = {40, 50, 5},
    .out_of_bounds_stiffness = 500,
    .out_of_bounds_bounce_damp = 0.0,
    
    // SPATIAL GRID
    .n_grid_cells_x = 10,
    .n_grid_cells_y = 10,
    .n_grid_cells_z = 10,
    .n_grid_cells_total = 1000,

    // SPAWN BOX
    .spawn_box_pos = {0, -20, 0},
    .spawn_box_dims = {40, 10, 5},
    
    // PARTICLES PARAMETERS
    .n_particles = 7,
    .grav_scale = 10.0,
    .particle_mass = 1.0,
    .particle_radius = 5.0,
    .stiffness_k = 20,
    .stiffness_gamma = 3.0,
    .reference_density = 0.05,
    .air_drag = 0.01
};
fluid_render_paramters fluid_render_params = {
    .render_radius = 0.5
};

//                                        SCENE FUNCTIONS
//                                             SETUP


//                         -------------------      SCENE SETUP AND LOOP       --------------------
void fluid_test_scene_setup(GLFWwindow* window){
    // CAMERA
    camera_setup(&camera_1);

    // FLUID SIM
    fluid_sim_setup(&fluid_sim_params);

    // FLUID RENDERER
    fluid_renderer_setup(&fluid_render_params, &fluid_sim_params);
    
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

    //  FLUID SIM
    // NEXT SIM STEP
    fluid_sim_params.delta_time = delta_time;
    pause_sim(window, &fluid_sim_params);
    one_sim_step(&fluid_sim_params);

    // CAMERA
    camera_loop(window, &camera_1, &mouse_x, &mouse_y, delta_time);
    
    //  FLUID RENDERER
    // DRAW BOUNDING
    fluid_renderer_loop_draw_bounding(&camera_1, &fluid_render_params, &fluid_sim_params);

    // DRAW PARTICLES
    fluid_renderer_loop_draw_fluid_particles(&camera_1, &fluid_render_params, &fluid_sim_params);
    
}   

void fluid_test_scene_end(GLFWwindow* window){
    // DO FREE FLUID SEEM AND RENDER
}