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

//                                            DATA
//                                      FLUID PARTICLES 
// ONE PARTICLE
float* fluid_particle_vertices; 
int fluid_particle_n_vertices;

unsigned int* fluid_particle_indices;
int fluid_particle_n_indices;

float fluid_particle_render_radius = 0.5;
float fluid_particle_mass = 1;
float fluid_particle_radius = 5;

// ALL PARTICLES
#define N_FLUID_SIM_PARTICLES 2
const int n_fluid_particles = N_FLUID_SIM_PARTICLES;
vec3  fluid_particle_positions[N_FLUID_SIM_PARTICLES];
vec3 fluid_particle_velocities[N_FLUID_SIM_PARTICLES];
float fluid_particle_densities[N_FLUID_SIM_PARTICLES];
float fluid_particle_pressures[N_FLUID_SIM_PARTICLES];
float grav_scale = 10.0;
float fluid_particles_stiffness_k = 200;
float fluid_particles_stiffness_gamma = 3;
float fluid_sim_reference_density = 0.05;
float fluid_sim_air_drag = 0.01; // between 0 and 1
float fluid_sim_out_of_bounds_bounce_damp = 0.0; // More like a wall friction coefficient - between 0 and 1

//                                      SIMULATION BOUNDING
vec3 bound_pos = {0,0,0};
vec3 bound_dims = {40,50,5};
float fluid_sim_out_of_bounds_stiffness = 500;

float* fluid_sim_bounding_vertices;
int fluid_sim_bounding_n_vertices;

unsigned int* fluid_sim_bounding_indices;
int fluid_sim_bounding_n_indices;

//                                      SPAWNING PARTICLES BOX
vec3 spawn_box_pos = {0,-20,0};
vec3 spawn_box_dims = {40,10,5};

//                                      SPATIAL PARTITIONING GRID
#define FLUID_SIM_N_GRID_CELLS_X 10
#define FLUID_SIM_N_GRID_CELLS_Y 10
#define FLUID_SIM_N_GRID_CELLS_Z 10
int fluid_sim_n_grid_cells_x = FLUID_SIM_N_GRID_CELLS_X;
int fluid_sim_n_grid_cells_y = FLUID_SIM_N_GRID_CELLS_Y;
int fluid_sim_n_grid_cells_z = FLUID_SIM_N_GRID_CELLS_Z;
int fluid_sim_n_grid_cells_total = FLUID_SIM_N_GRID_CELLS_X * FLUID_SIM_N_GRID_CELLS_Y * FLUID_SIM_N_GRID_CELLS_Z;
int fluid_sim_grid_particle_cells[N_FLUID_SIM_PARTICLES];
int fluid_sim_grid_num_particles_prefix_sums[FLUID_SIM_N_GRID_CELLS_X * FLUID_SIM_N_GRID_CELLS_Y * FLUID_SIM_N_GRID_CELLS_Z];
int fluid_sim_grid[N_FLUID_SIM_PARTICLES];

fluid_sim_parameters fluid_sim_params;
fluid_render_paramters fluid_render_params;

//                                        SCENE FUNCTIONS
//                                             SETUP

static void setup_fluid_sim_data(){
    //                                  INTITALIZE FLUID SIM PARAMS
    
    // BOUNDARIES
    glm_vec3_copy(bound_pos, fluid_sim_params.bound_pos);
    glm_vec3_copy(bound_dims, fluid_sim_params.bound_dims);
    fluid_sim_params.out_of_bounds_stiffness = fluid_sim_out_of_bounds_stiffness;
    fluid_sim_params.out_of_bounds_bounce_damp = fluid_sim_out_of_bounds_bounce_damp; 
    
    // SPATIAL GRID
    fluid_sim_params.n_grid_cells_x = fluid_sim_n_grid_cells_x;
    fluid_sim_params.n_grid_cells_y = fluid_sim_n_grid_cells_y;
    fluid_sim_params.n_grid_cells_z = fluid_sim_n_grid_cells_z;
    fluid_sim_params.n_grid_cells_total = fluid_sim_n_grid_cells_total;
    fluid_sim_params.grid_particle_cells = &fluid_sim_grid_particle_cells[0];
    fluid_sim_params.grid_cells_num_particles_prefix_sums = &fluid_sim_grid_num_particles_prefix_sums[0];
    fluid_sim_params.grid = &fluid_sim_grid[0];

    // SPAWN BOX
    glm_vec3_copy(spawn_box_pos, fluid_sim_params.spawn_box_pos);
    glm_vec3_copy(spawn_box_dims, fluid_sim_params.spawn_box_dims);
    
    // PARTICLES PARAMETERS
    fluid_sim_params.n_particles = n_fluid_particles;
    fluid_sim_params.grav_scale = grav_scale;
    fluid_sim_params.particle_mass = fluid_particle_mass;
    fluid_sim_params.particle_radius = fluid_particle_radius;
    fluid_sim_params.stiffness_k = fluid_particles_stiffness_k;
    fluid_sim_params.stiffness_gamma = fluid_particles_stiffness_gamma;
    fluid_sim_params.reference_density = fluid_sim_reference_density;
    fluid_sim_params.air_drag = fluid_sim_air_drag;


    // SIM STATE
    fluid_sim_params.delta_time = 0;
    fluid_sim_params.is_running = 0;
    fluid_sim_params.positions = &fluid_particle_positions[0];
    fluid_sim_params.velocities = &fluid_particle_velocities[0];
    fluid_sim_params.densities = &fluid_particle_densities[0];
    fluid_sim_params.pressures = &fluid_particle_pressures[0];

    // ALL PARTICLES
    setup_particle_positions_in_box(&fluid_sim_params);
    setup_particle_velocities(&fluid_sim_params);
    setup_particle_densities(&fluid_sim_params);
    setup_sim_grid(&fluid_sim_params);
}

static void setup_fluid_render_data(){
    
    // the only thing the user should have to manuallyy set is the rendering radius of particles
    // the rest is set up by fluid_renderer_setup()
    fluid_render_params.render_radius = fluid_particle_render_radius;
    
}

static void setup_data(){
    

    // ONE PARTICLE
    fluid_particle_n_vertices = get_prim_cube_n_vertices();
    fluid_particle_n_indices = get_prim_cube_n_indices();

    fluid_particle_vertices = malloc(fluid_particle_n_vertices * 3 * sizeof(float));
    fluid_particle_indices = malloc(fluid_particle_n_indices * sizeof(unsigned int));
    get_prim_cube(fluid_particle_vertices, fluid_particle_indices, fluid_particle_render_radius);

    // SIMULATION BOUNDINGS
    fluid_sim_bounding_n_vertices = get_prim_rectangle_n_vertices();
    fluid_sim_bounding_n_indices = get_prim_cube_n_indices();

    fluid_sim_bounding_vertices = malloc(fluid_sim_bounding_n_vertices * 3 * sizeof(float));
    fluid_sim_bounding_indices = malloc(fluid_sim_bounding_n_indices * sizeof(unsigned int));
    get_prim_rectangle(fluid_sim_bounding_vertices, fluid_sim_bounding_indices, bound_dims[0], bound_dims[2], bound_dims[1]);

    setup_fluid_sim_data();
    setup_fluid_render_data();

}

/* static void pass_data_to_fluid_renderer(){
    fluid_renderer_pass_bounding_data(fluid_sim_bounding_vertices, fluid_sim_bounding_n_vertices, fluid_sim_bounding_indices, fluid_sim_bounding_n_indices);
    fluid_renderer_pass_one_particle_data(fluid_particle_vertices, fluid_particle_n_vertices, fluid_particle_indices, fluid_particle_n_indices);
} */

//                         -------------------      SCENE SETUP AND LOOP       --------------------
void fluid_test_scene_setup(GLFWwindow* window){
    // DATA
    setup_data();
    //setup_buffers();

    // CAMERA
    camera_setup(&camera_1);

    // FLUID RENDERER
    //pass_data_to_fluid_renderer();
    fluid_renderer_setup(&fluid_render_params, &fluid_sim_params);
    
    
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

    // NEXT SIM STEP
    fluid_sim_params.delta_time = delta_time;
    pause_sim(window, &fluid_sim_params);
    one_sim_step(&fluid_sim_params);

    // CAMERA
    camera_loop(window, &camera_1, &mouse_x, &mouse_y, delta_time);
    
    // FLUID RENDERER
    // DRAW BOUNDING
    fluid_renderer_loop_draw_bounding(&camera_1, &fluid_render_params, &fluid_sim_params);

    // DRAW PARTICLES
    fluid_renderer_loop_draw_fluid_particles(&camera_1, &fluid_render_params, &fluid_sim_params);

}   

void fluid_test_scene_end(GLFWwindow* window){
    free_prim_rectangle(fluid_sim_bounding_vertices, fluid_sim_bounding_indices);
    free_prim_plane(fluid_particle_vertices, fluid_particle_indices);
}