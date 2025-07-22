#include "fluid_test_scene.h"

//                                  SCENE VARIABLES
//                                      WORLD
vec3 x_axis = {1,0,0};
vec3 y_axis = {0,1,0};
vec3 z_axis = {0,0,1};
mat4 view, projection;

//                                          CAMERA
float camera_fov = 100.0;
float camera_aspect = 1;
float camera_near = 0.01;
float camera_far = 1000.0;
float camera_pitch = 0.0;
float camera_yaw = 0.0;
float camera_move_speed = 10;
float camera_look_speed = 20;
float camera_zoom_speed = 1;
vec3 camera_pos = {0, 0, -10};
vec3 camera_dir = {0, 1, 0};


//                                          ENGINE 
float delta_time = 1.0;
float last_frame_time = 0.0;
float mouse_x = 400;
float mouse_y = 400;


//                                  SHADERS AND SHADER PROGRAMS
unsigned int particle_vertex_shader, particle_fragment_shader;
unsigned int particle_shader_program;

unsigned int bounding_vertex_shader, bounding_fragment_shader;
unsigned int bounding_shader_program;

//                                      BUFFERS / TEXTURES

unsigned int fluid_particle_vertices_VBO, fluid_particles_pos_VBO, fluid_particles_densities_VBO, fluid_particles_pressures_VBO;
unsigned int VAO;
unsigned int fluid_particle_IBO;

unsigned int fluid_sim_bounding_vertices_VBO;
unsigned int fluid_sim_bounding_IBO;

//                                            DATA
//                                      FLUID PARTICLES 
// ONE PARTICLE
float* fluid_particle_vertices; 
int fluid_particle_n_vertices;

unsigned int* fluid_particle_indices;
int fluid_particle_n_indices;

float fluid_particle_render_radius = 0.2;
float fluid_particle_mass = 0.5;
float fluid_particle_radius = 2;

// ALL PARTICLES

const  int n_fluid_particles = 400;
vec3  fluid_particle_positions[400];
vec3 fluid_particle_velocities[400];
float fluid_particle_densities[400];
float fluid_particle_pressures[400];
float grav_scale = 10.0;
float fluid_particles_stiffness_k = 5;
float fluid_particles_stiffness_gamma = 3;
float fluid_sim_reference_density = 0.1;
float fluid_sim_air_drag = 0.01; // between 0 and 1
float fluid_sim_out_of_bounds_bounce_damp = 0.02; // More like a wall friction coefficient - between 0 and 1

//                                      SIMULATION BOUNDING
vec3 bound_pos = {0,0,0};
vec3 bound_dims = {10,10,2};
float fluid_sim_out_of_bounds_stiffness = 100;



float* fluid_sim_bounding_vertices;
int fluid_sim_bounding_n_vertices;

unsigned int* fluid_sim_bounding_indices;
int fluid_sim_bounding_n_indices;

//                                      SPAWNING PARTICLES BOX
vec3 spawn_box_pos = {0,0,0};
vec3 spawn_box_dims = {5,5,2};

fluid_sim_parameters fluid_sim_params;



//                                        SCENE FUNCTIONS
//                                             SETUP
static void setup_shaders(){ // and shader programs
    // PARTICLES SHADERS
    const char* particle_vertex_shader_source = get_shader_content("./shaders/main_pipeline/fluid_test_scene/particles/particle_vertex_shader.glsl");
    const char* particle_fragment_shader_source = get_shader_content("./shaders/main_pipeline/fluid_test_scene/particles/particle_fragment_shader.glsl");
    
    setup_fvc_shader(&particle_vertex_shader, &particle_vertex_shader_source, "vertex");
    fvc_shader_comp_error(&particle_vertex_shader, "vertex");
    
    setup_fvc_shader(&particle_fragment_shader, &particle_fragment_shader_source, "fragment");
    fvc_shader_comp_error(&particle_fragment_shader, "fragment");

    // PARTICLES SHADERS PROGRAM
    setup_fvc_shader_prog(&particle_shader_program, &particle_fragment_shader, &particle_vertex_shader, NULL, "main_pipeline");
    fvc_shader_prog_link_error(&particle_shader_program, "main_pipeline");
    
    // BOUNDING SHADERS
    const char* bounding_vertex_shader_source = get_shader_content("./shaders/main_pipeline/fluid_test_scene/sim_bounding/bounding_vertex_shader.glsl");

    const char* bounding_fragment_shader_source = get_shader_content("./shaders/main_pipeline/fluid_test_scene/sim_bounding/bounding_fragment_shader.glsl");
    
    setup_fvc_shader(&bounding_vertex_shader, &bounding_vertex_shader_source, "vertex");
    fvc_shader_comp_error(&bounding_vertex_shader, "vertex");
    
    setup_fvc_shader(&bounding_fragment_shader, &bounding_fragment_shader_source, "fragment");
    fvc_shader_comp_error(&bounding_fragment_shader, "fragment");

    // BOUNDING SHADER PROGRAM
    setup_fvc_shader_prog(&bounding_shader_program, &bounding_fragment_shader, &bounding_vertex_shader, NULL, "main_pipeline");
    fvc_shader_prog_link_error(&bounding_shader_program, "main_pipeline");
}

static void setup_buffers(){
    // VAO
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    
    // ONE FLUID PARTICLE
    glGenBuffers(1, &fluid_particle_vertices_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, fluid_particle_vertices_VBO);
    glBufferData(GL_ARRAY_BUFFER, fluid_particle_n_vertices*3*sizeof(float), fluid_particle_vertices, GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    // ALL FLUID PARTICLES
    glGenBuffers(1, &fluid_particles_pos_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, fluid_particles_pos_VBO);
    glBufferData(GL_ARRAY_BUFFER, n_fluid_particles * 3 * sizeof(float), fluid_particle_positions , GL_DYNAMIC_DRAW);
    
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribDivisor(1, 1);

    // DENSITIES
    glGenBuffers(1, &fluid_particles_densities_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, fluid_particles_densities_VBO);
    glBufferData(GL_ARRAY_BUFFER, n_fluid_particles * sizeof(float), fluid_particle_densities , GL_DYNAMIC_DRAW);
    
    glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(float), (void*)0);
    glEnableVertexAttribArray(3);
    glVertexAttribDivisor(3, 1);

    // PRESSURES
    glGenBuffers(1, &fluid_particles_pressures_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, fluid_particles_pressures_VBO);
    glBufferData(GL_ARRAY_BUFFER, n_fluid_particles * sizeof(float), fluid_particle_densities , GL_DYNAMIC_DRAW);
    
    glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, sizeof(float), (void*)0);
    glEnableVertexAttribArray(4);
    glVertexAttribDivisor(4, 1);

    // ONE FLUID PARTICLE IBO
    glGenBuffers(1, &fluid_particle_IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, fluid_particle_IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, fluid_particle_n_indices*sizeof(unsigned), fluid_particle_indices, GL_DYNAMIC_DRAW);

    // SIMULATION BOUNDINGS
    glGenBuffers(1, &fluid_sim_bounding_vertices_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, fluid_sim_bounding_vertices_VBO);
    glBufferData(GL_ARRAY_BUFFER, fluid_sim_bounding_n_vertices * 3 * sizeof(float), fluid_sim_bounding_vertices, GL_DYNAMIC_DRAW);

    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(2);

    // SIMULATION BOUNDING IBO
    glGenBuffers(1, &fluid_sim_bounding_IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, fluid_sim_bounding_IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, fluid_sim_bounding_n_indices*sizeof(unsigned), fluid_sim_bounding_indices, GL_DYNAMIC_DRAW);
}

static void setup_data(){

    //                                  INTITALIZE FLUID SIM PARAMS
    
    // BOUNDARIES
    glm_vec3_copy(bound_pos, fluid_sim_params.bound_pos);
    glm_vec3_copy(bound_dims, fluid_sim_params.bound_dims);
    fluid_sim_params.out_of_bounds_stiffness = fluid_sim_out_of_bounds_stiffness;
    fluid_sim_params.out_of_bounds_bounce_damp = fluid_sim_out_of_bounds_bounce_damp; 
    
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
    
    // ---------
    
    // ALL PARTICLES
    setup_particle_positions_in_box(&fluid_sim_params);
    setup_particle_velocities(&fluid_sim_params);
    setup_particle_densities(&fluid_sim_params);

    // ONE PARTICLE
    fluid_particle_n_vertices = get_prim_plane_n_vertices();
    fluid_particle_n_indices = get_prim_rectangle_n_indices();

    fluid_particle_vertices = malloc(fluid_particle_n_vertices * 3 * sizeof(float));
    fluid_particle_indices = malloc(fluid_particle_n_indices * sizeof(unsigned int));
    get_prim_plane(fluid_particle_vertices, fluid_particle_indices, fluid_particle_render_radius);

    // SIMULATION BOUNDINGS
    fluid_sim_bounding_n_vertices = get_prim_rectangle_n_vertices();
    fluid_sim_bounding_n_indices = get_prim_cube_n_indices();

    fluid_sim_bounding_vertices = malloc(fluid_sim_bounding_n_vertices * 3 * sizeof(float));
    fluid_sim_bounding_indices = malloc(fluid_sim_bounding_n_indices * sizeof(unsigned int));
    get_prim_rectangle(fluid_sim_bounding_vertices, fluid_sim_bounding_indices, bound_dims[0], bound_dims[2], bound_dims[1]);

    

}

static void setup_camera_and_matrices(){
    // CAMERA
    //glm_vec3_zero(camera_pos);
    //glm_vec3_zero(camera_dir);

    // VIEW AND PROJECTION
    glm_mat4_identity(view);
    vec3 target;
    glm_vec3_add(camera_pos, camera_dir, target);
    
    glm_translate(view, camera_pos);
    glm_lookat(camera_pos, target, y_axis, view);

    glm_perspective(glm_rad(camera_fov), camera_aspect, camera_near, camera_far, projection);
}

//                                           GAME LOOP
static void loop_camera_and_matrices(GLFWwindow* window){
    // UPDATE THE TRANSFORM MATRICES
    glm_mat4_identity(view); // RESET VIEW MATRIX AFTER EACH FRAME BEFORE UPDATING CAMERA
    camera_3d_angles_update(window, camera_dir, camera_look_speed, &camera_pitch, &camera_yaw, &mouse_x, &mouse_y, delta_time);
    camera_3d_move_update(window, camera_pos, camera_dir, view, camera_move_speed, delta_time);
    camera_3d_direction_update(window, camera_pos, camera_dir, view, camera_pitch, camera_yaw);
}

//                                         SIM BOUNDING
static void loop_bounding_buffers(){
    // SIMULATION BOUNDINGS
    
    glBindBuffer(GL_ARRAY_BUFFER, fluid_sim_bounding_vertices_VBO);
    glBufferData(GL_ARRAY_BUFFER, fluid_sim_bounding_n_vertices * 3 * sizeof(float), fluid_sim_bounding_vertices, GL_DYNAMIC_DRAW);

    // SIMULATION BOUNDINGS IBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, fluid_sim_bounding_IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, fluid_sim_bounding_n_indices*sizeof(unsigned), fluid_sim_bounding_indices, GL_DYNAMIC_DRAW);

}

static void loop_draw_bounding(){

    // USE APPROPRIATE SHADER PROGRAM
    glUseProgram(bounding_shader_program);

    // BOUNDING BUFFER
    loop_bounding_buffers();

    // PASSING UNIFORMS
    int viewLoc = glGetUniformLocation(bounding_shader_program, "view");
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, view[0]);
    int projectionLoc = glGetUniformLocation(bounding_shader_program, "projection");
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, projection[0]);

    // DRAWING
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, fluid_sim_bounding_vertices_VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, fluid_sim_bounding_IBO);
    glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
    glDrawElements(GL_TRIANGLES, fluid_sim_bounding_n_indices, GL_UNSIGNED_INT, 0);

}

//                                       FLUID PARTICLES
static void loop_fluid_particles_buffers(){
    // ONE FLUID PARTICLE
    
    glBindBuffer(GL_ARRAY_BUFFER, fluid_particle_vertices_VBO);
    glBufferData(GL_ARRAY_BUFFER, fluid_particle_n_vertices*3*sizeof(float), fluid_particle_vertices, GL_DYNAMIC_DRAW);
    
    // ALL FLUID PARTICLES
    
    glBindBuffer(GL_ARRAY_BUFFER, fluid_particles_pos_VBO);
    glBufferData(GL_ARRAY_BUFFER, n_fluid_particles * 3 * sizeof(float), fluid_particle_positions , GL_DYNAMIC_DRAW);

    // DENSITIES
    glBindBuffer(GL_ARRAY_BUFFER, fluid_particles_densities_VBO);
    glBufferData(GL_ARRAY_BUFFER, n_fluid_particles * sizeof(float), fluid_particle_densities , GL_DYNAMIC_DRAW);

    // PRESSURES
    glBindBuffer(GL_ARRAY_BUFFER, fluid_particles_pressures_VBO);
    glBufferData(GL_ARRAY_BUFFER, n_fluid_particles * sizeof(float), fluid_particle_densities , GL_DYNAMIC_DRAW);

    // ONE FLUID PARTICLE IBO
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, fluid_particle_IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, fluid_particle_n_indices*sizeof(unsigned), fluid_particle_indices, GL_DYNAMIC_DRAW);

}

static void loop_draw_fluid_particles(){
    // USE APPROPRIATE SHADER PROGRAM
    glUseProgram(particle_shader_program);

    // BOUNDING BUFFER
    loop_fluid_particles_buffers();

    // PASSING UNIFORMS
    int viewLoc = glGetUniformLocation(particle_shader_program, "view");
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, view[0]);
    int projectionLoc = glGetUniformLocation(particle_shader_program, "projection");
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, projection[0]);
    int particleRadiusLoc = glGetUniformLocation(particle_shader_program, "particle_radius");
    glUniform1f(particleRadiusLoc, fluid_particle_render_radius);

    // DRAWING
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, fluid_particle_vertices_VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, fluid_particle_IBO);
    glBindBuffer(GL_ARRAY_BUFFER, fluid_particles_pos_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, fluid_particles_densities_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, fluid_particles_pressures_VBO);
    
    
    glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
    glDrawElementsInstanced(GL_TRIANGLES, fluid_particle_n_indices, GL_UNSIGNED_INT, 0, n_fluid_particles);
}

//                                      SCENE SETUP AND LOOP
void fluid_test_scene_setup(GLFWwindow* window){
    setup_shaders();
    setup_camera_and_matrices();
    setup_data();
    setup_buffers();
    
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

    // APPLY SIMULATION STEP
    fluid_sim_params.delta_time = delta_time;
    pause_sim(window, &fluid_sim_params);
    one_sim_step(&fluid_sim_params);

    
    loop_camera_and_matrices(window);
    
    // DRAW BOUNDING
    
    loop_draw_bounding();

    // DRAW PARTICLES

    loop_draw_fluid_particles();


}   

void fluid_test_scene_end(GLFWwindow* window){
    free_prim_rectangle(fluid_sim_bounding_vertices, fluid_sim_bounding_indices);
    free_prim_plane(fluid_particle_vertices, fluid_particle_indices);
}