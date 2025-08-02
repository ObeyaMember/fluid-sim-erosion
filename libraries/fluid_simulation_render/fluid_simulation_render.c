#include "fluid_simulation_render.h"

//                                                              DATA
// ONE PARTICLE
static float* fluid_particle_vertices; 
static int fluid_particle_n_vertices;

static unsigned int* fluid_particle_indices;
static int fluid_particle_n_indices;

// ALL PARTICLES
int n_particles = 0;
 
 
// SIM BOUNDING
static float* fluid_sim_bounding_vertices;
static int fluid_sim_bounding_n_vertices;
 
static unsigned int* fluid_sim_bounding_indices;
static int fluid_sim_bounding_n_indices;

//                                                  SHADERS AND SHADER PROGRAMS
unsigned int particle_vertex_shader, particle_fragment_shader;
unsigned int particle_shader_program;

unsigned int bounding_vertex_shader, bounding_fragment_shader;
unsigned int bounding_shader_program;


//                                                              BUFFERS
static unsigned int fluid_particle_vertices_VBO, fluid_particles_pos_VBO, fluid_particles_densities_VBO, fluid_particles_pressures_VBO;
static unsigned int VAO;
static unsigned int fluid_particle_IBO;
 
static unsigned int fluid_sim_bounding_vertices_VBO;
static unsigned int fluid_sim_bounding_IBO;


// MAKE A STRUCT FOR THESE PARAMS AT SOME POINT - too lazy rn


//                                                                  SETUP

// NEED TO PASS SHAPES DATA
void fluid_renderer_pass_one_particle_data(float* particle_vertices, int particle_n_vertices, unsigned int* particle_indices, int particle_n_indices){
    fluid_particle_vertices = particle_vertices; 
    fluid_particle_n_vertices = particle_n_vertices;

    fluid_particle_indices = particle_indices;
    fluid_particle_n_indices = particle_n_indices;
}

void fluid_renderer_pass_bounding_data(float* sim_bounding_vertices, int sim_bounding_n_vertices, unsigned int* sim_bounding_indices, int sim_bounding_n_indices){
    fluid_sim_bounding_vertices = sim_bounding_vertices;
    fluid_sim_bounding_n_vertices = sim_bounding_n_vertices;

    fluid_sim_bounding_indices = sim_bounding_indices;
    fluid_sim_bounding_n_indices = sim_bounding_n_indices;
    printf("a\n");
}

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


static void setup_particles_buffers(fluid_sim_parameters* sim_params){
    n_particles = sim_params->n_particles;
    vec3* fluid_particle_positions = sim_params->positions;

    // ONE FLUID PARTICLE
    glGenBuffers(1, &fluid_particle_vertices_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, fluid_particle_vertices_VBO);
    glBufferData(GL_ARRAY_BUFFER, fluid_particle_n_vertices*3*sizeof(float), fluid_particle_vertices, GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // ONE FLUID PARTICLE IBO
    glGenBuffers(1, &fluid_particle_IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, fluid_particle_IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, fluid_particle_n_indices*sizeof(unsigned), fluid_particle_indices, GL_DYNAMIC_DRAW);
    
    // ALL FLUID PARTICLES
    glGenBuffers(1, &fluid_particles_pos_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, fluid_particles_pos_VBO);
    glBufferData(GL_ARRAY_BUFFER, n_particles * 3 * sizeof(float), fluid_particle_positions , GL_DYNAMIC_DRAW);
    
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribDivisor(1, 1);
}

static void setup_fluid_data_buffers(fluid_sim_parameters* sim_params){
    n_particles = sim_params->n_particles;
    float* fluid_particle_densities = sim_params->densities;
    float* fluid_particle_pressures = sim_params->pressures;

    // DENSITIES
    glGenBuffers(1, &fluid_particles_densities_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, fluid_particles_densities_VBO);
    glBufferData(GL_ARRAY_BUFFER, n_particles * sizeof(float), fluid_particle_densities , GL_DYNAMIC_DRAW);
    
    glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(float), (void*)0);
    glEnableVertexAttribArray(3);
    glVertexAttribDivisor(3, 1);

    // PRESSURES
    glGenBuffers(1, &fluid_particles_pressures_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, fluid_particles_pressures_VBO);
    glBufferData(GL_ARRAY_BUFFER, n_particles * sizeof(float), fluid_particle_pressures , GL_DYNAMIC_DRAW);
    
    glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, sizeof(float), (void*)0);
    glEnableVertexAttribArray(4);
    glVertexAttribDivisor(4, 1);
}

static void setup_bounding_buffers(){
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

static void setup_VAO(){
    // VAO
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
}

void fluid_renderer_setup(fluid_sim_parameters* sim_params){
    setup_shaders();
    setup_VAO();
    setup_particles_buffers(sim_params);
    setup_fluid_data_buffers(sim_params);
    setup_bounding_buffers();
    
}

//                                                                 MAIN LOOP
// BOUNDING
static void loop_bounding_buffers(){
    // SIMULATION BOUNDINGS
    
    glBindBuffer(GL_ARRAY_BUFFER, fluid_sim_bounding_vertices_VBO);
    glBufferData(GL_ARRAY_BUFFER, fluid_sim_bounding_n_vertices * 3 * sizeof(float), fluid_sim_bounding_vertices, GL_DYNAMIC_DRAW);

    // SIMULATION BOUNDINGS IBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, fluid_sim_bounding_IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, fluid_sim_bounding_n_indices*sizeof(unsigned), fluid_sim_bounding_indices, GL_DYNAMIC_DRAW);

}

void fluid_renderer_loop_draw_bounding(mat4 view, mat4 projection, fluid_sim_parameters* sim_params){
    //printf("baound_vertices[0]: %f\n", fluid_sim_bounding_vertices[0]);
    vec3 bound_pos;
    glm_vec3_copy(sim_params->bound_pos, bound_pos);

    // USE APPROPRIATE SHADER PROGRAM
    glUseProgram(bounding_shader_program);

    // BOUNDING BUFFER
    loop_bounding_buffers();

    // PASSING UNIFORMS
    int viewLoc = glGetUniformLocation(bounding_shader_program, "view");
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, view[0]);
    int projectionLoc = glGetUniformLocation(bounding_shader_program, "projection");
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, projection[0]);
    int boundingPosLoc = glGetUniformLocation(bounding_shader_program, "bounding_pos");
    glUniform3fv(boundingPosLoc, 1, bound_pos);

    //printf("sim bounding n indices: %d\n", fluid_sim_bounding_n_indices);
    //printf("sim bounding indices[35]: %d\n", fluid_sim_bounding_indices[35]);
    // DRAWING
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, fluid_sim_bounding_vertices_VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, fluid_sim_bounding_IBO);
    glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
    glDrawElements(GL_TRIANGLES, fluid_sim_bounding_n_indices, GL_UNSIGNED_INT, 0);

}

// PARTICLES

static void loop_particles_buffers(fluid_sim_parameters* sim_params){
    vec3* fluid_particle_positions = sim_params->positions;

    // ONE FLUID PARTICLE
    glBindBuffer(GL_ARRAY_BUFFER, fluid_particle_vertices_VBO);
    glBufferData(GL_ARRAY_BUFFER, fluid_particle_n_vertices*3*sizeof(float), fluid_particle_vertices, GL_DYNAMIC_DRAW);
    
    // ALL FLUID PARTICLES
    glBindBuffer(GL_ARRAY_BUFFER, fluid_particles_pos_VBO);
    glBufferData(GL_ARRAY_BUFFER, n_particles * 3 * sizeof(float), fluid_particle_positions , GL_DYNAMIC_DRAW);

    // ONE FLUID PARTICLE IBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, fluid_particle_IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, fluid_particle_n_indices*sizeof(unsigned), fluid_particle_indices, GL_DYNAMIC_DRAW);
}

static void loop_fluid_data_buffers(fluid_sim_parameters* sim_params){
    float* fluid_particle_densities = sim_params->densities;
    float* fluid_particle_pressures = sim_params->pressures;

    // DENSITIES
    glBindBuffer(GL_ARRAY_BUFFER, fluid_particles_densities_VBO);
    glBufferData(GL_ARRAY_BUFFER, n_particles * sizeof(float), fluid_particle_densities , GL_DYNAMIC_DRAW);

    // PRESSURES
    glBindBuffer(GL_ARRAY_BUFFER, fluid_particles_pressures_VBO);
    glBufferData(GL_ARRAY_BUFFER, n_particles * sizeof(float), fluid_particle_densities , GL_DYNAMIC_DRAW);

}

static void loop_fluid_buffers(fluid_sim_parameters* sim_params){
    loop_particles_buffers(sim_params);
    loop_fluid_data_buffers(sim_params);
}

void fluid_renderer_loop_draw_fluid_particles(mat4 view, mat4 projection, float particle_render_radius, fluid_sim_parameters* sim_params){
    // USE APPROPRIATE SHADER PROGRAM
    glUseProgram(particle_shader_program);

    // FLUID PARTICLES BUFFER
    loop_fluid_buffers(sim_params);

    // PASSING UNIFORMS
    int viewLoc = glGetUniformLocation(particle_shader_program, "view");
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, view[0]);
    int projectionLoc = glGetUniformLocation(particle_shader_program, "projection");
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, projection[0]);
    int particleRadiusLoc = glGetUniformLocation(particle_shader_program, "particle_radius");
    glUniform1f(particleRadiusLoc, particle_render_radius);

    // DRAWING
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, fluid_particle_vertices_VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, fluid_particle_IBO);
    glBindBuffer(GL_ARRAY_BUFFER, fluid_particles_pos_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, fluid_particles_densities_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, fluid_particles_pressures_VBO);
    
    
    glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
    glDrawElementsInstanced(GL_TRIANGLES, fluid_particle_n_indices, GL_UNSIGNED_INT, 0, n_particles);
}

