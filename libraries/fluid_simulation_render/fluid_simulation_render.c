#include "fluid_simulation_render.h"

//                                                                  SETUP

// DATA
static void setup_one_particle_data(fluid_render_paramters* render_params){
    
    render_params->fluid_particle_n_vertices = get_prim_cube_n_vertices();
    render_params->fluid_particle_n_indices = get_prim_cube_n_indices();

    render_params->fluid_particle_vertices = malloc(render_params->fluid_particle_n_vertices * 3 * sizeof(float));
    render_params->fluid_particle_indices = malloc(render_params->fluid_particle_n_indices * sizeof(unsigned int));
    get_prim_cube(render_params->fluid_particle_vertices, render_params->fluid_particle_indices, render_params->render_radius);
}

static void setup_bounding_data(fluid_render_paramters* render_params, fluid_sim_parameters* sim_params){
    // SIMULATION BOUNDINGS
    render_params->fluid_sim_bounding_n_vertices = get_prim_rectangle_n_vertices();
    render_params->fluid_sim_bounding_n_indices = get_prim_cube_n_indices();

    render_params->fluid_sim_bounding_vertices = malloc(render_params->fluid_sim_bounding_n_vertices * 3 * sizeof(float));
    render_params->fluid_sim_bounding_indices = malloc(render_params->fluid_sim_bounding_n_indices * sizeof(unsigned int));
    get_prim_rectangle(render_params->fluid_sim_bounding_vertices, render_params->fluid_sim_bounding_indices, sim_params->bound_dims[0], sim_params->bound_dims[2], sim_params->bound_dims[1]);
}

static void setup_sim_grid_data(fluid_render_paramters* render_params, fluid_sim_parameters* sim_params){
    render_params->fluid_sim_grid_cell_n_vertices = get_prim_rectangle_n_vertices();
    render_params->fluid_sim_grid_cell_n_indices = get_prim_rectangle_n_indices();

    render_params->fluid_sim_grid_cell_vertices = malloc(render_params->fluid_sim_grid_cell_n_vertices * 3 * sizeof(float));
    render_params->fluid_sim_grid_cell_indices = malloc(render_params->fluid_sim_grid_cell_n_indices * sizeof(unsigned int));
    float l, w, h;
    l = sim_params->bound_dims[0] / (sim_params->n_grid_cells_x);
    h = sim_params->bound_dims[1] / (sim_params->n_grid_cells_y);
    w = sim_params->bound_dims[2] / (sim_params->n_grid_cells_z);
    get_prim_rectangle(render_params->fluid_sim_grid_cell_vertices, render_params->fluid_sim_grid_cell_indices, l / 1.1, w / 1.1, h / 1.1);

    vec3 grid_cell_dims = {l, h, w};
    glm_vec3_copy(grid_cell_dims, render_params->fluid_sim_grid_cell_dims);

}

// SHADERS
static void setup_shaders(fluid_render_paramters* render_params){ // and shader programs
    // PARTICLES SHADERS
    const char* particle_vertex_shader_source = get_shader_content("./shaders/main_pipeline/fluid_test_scene/particles/particle_vertex_shader.glsl");
    const char* particle_fragment_shader_source = get_shader_content("./shaders/main_pipeline/fluid_test_scene/particles/particle_fragment_shader.glsl");
    
    setup_fvc_shader(&render_params->particle_vertex_shader, &particle_vertex_shader_source, "vertex");
    fvc_shader_comp_error(&render_params->particle_vertex_shader, "vertex");
    
    setup_fvc_shader(&render_params->particle_fragment_shader, &particle_fragment_shader_source, "fragment");
    fvc_shader_comp_error(&render_params->particle_fragment_shader, "fragment");

    // PARTICLES SHADERS PROGRAM
    setup_fvc_shader_prog(&render_params->particle_shader_program, &render_params->particle_fragment_shader, &render_params->particle_vertex_shader, NULL, "main_pipeline");
    fvc_shader_prog_link_error(&render_params->particle_shader_program, "main_pipeline");
    
    // BOUNDING SHADERS
    const char* bounding_vertex_shader_source = get_shader_content("./shaders/main_pipeline/fluid_test_scene/sim_bounding/bounding_vertex_shader.glsl");
    const char* bounding_fragment_shader_source = get_shader_content("./shaders/main_pipeline/fluid_test_scene/sim_bounding/bounding_fragment_shader.glsl");
    
    setup_fvc_shader(&render_params->bounding_vertex_shader, &bounding_vertex_shader_source, "vertex");
    fvc_shader_comp_error(&render_params->bounding_vertex_shader, "vertex");
    
    setup_fvc_shader(&render_params->bounding_fragment_shader, &bounding_fragment_shader_source, "fragment");
    fvc_shader_comp_error(&render_params->bounding_fragment_shader, "fragment");

    // BOUNDING SHADER PROGRAM
    setup_fvc_shader_prog(&render_params->bounding_shader_program, &render_params->bounding_fragment_shader, &render_params->bounding_vertex_shader, NULL, "main_pipeline");
    fvc_shader_prog_link_error(&render_params->bounding_shader_program, "main_pipeline");

    // GRID SHADERS
    const char* grid_vertex_shader_source = get_shader_content("./shaders/main_pipeline/fluid_test_scene/sim_grid/sim_grid_vertex_shader.glsl");
    const char* grid_fragment_shader_source = get_shader_content("./shaders/main_pipeline/fluid_test_scene/sim_grid/sim_grid_fragment_shader.glsl");
    
    setup_fvc_shader(&render_params->grid_vertex_shader, &grid_vertex_shader_source, "vertex");
    fvc_shader_comp_error(&render_params->grid_vertex_shader, "vertex");
    
    setup_fvc_shader(&render_params->grid_fragment_shader, &grid_fragment_shader_source, "fragment");
    fvc_shader_comp_error(&render_params->grid_fragment_shader, "fragment");

    // GRID SHADER PROGRAM
    setup_fvc_shader_prog(&render_params->grid_shader_program, &render_params->grid_fragment_shader, &render_params->grid_vertex_shader, NULL, "main_pipeline");
    fvc_shader_prog_link_error(&render_params->grid_shader_program, "main_pipeline");
}

// BUFFERS
static void setup_particles_buffers(fluid_render_paramters* render_params, fluid_sim_parameters* sim_params){
    
    vec3* fluid_particle_positions = sim_params->positions;

    // ONE FLUID PARTICLE
    glGenBuffers(1, &render_params->fluid_particle_vertices_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, render_params->fluid_particle_vertices_VBO);
    glBufferData(GL_ARRAY_BUFFER, render_params->fluid_particle_n_vertices*3*sizeof(float), render_params->fluid_particle_vertices, GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // ONE FLUID PARTICLE IBO
    glGenBuffers(1, &render_params->fluid_particle_IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, render_params->fluid_particle_IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, render_params->fluid_particle_n_indices*sizeof(unsigned), render_params->fluid_particle_indices, GL_DYNAMIC_DRAW);
    
    // ALL FLUID PARTICLES
    glGenBuffers(1, &render_params->fluid_particles_pos_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, render_params->fluid_particles_pos_VBO);
    glBufferData(GL_ARRAY_BUFFER, sim_params->n_particles * 3 * sizeof(float), sim_params->positions , GL_DYNAMIC_DRAW);
    
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribDivisor(1, 1);
}

static void setup_fluid_data_buffers(fluid_render_paramters* render_params, fluid_sim_parameters* sim_params){
    
    float* fluid_particle_densities = sim_params->densities;
    float* fluid_particle_pressures = sim_params->pressures;

    // DENSITIES
    glGenBuffers(1, &render_params->fluid_particles_densities_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, render_params->fluid_particles_densities_VBO);
    glBufferData(GL_ARRAY_BUFFER, sim_params->n_particles * sizeof(float), sim_params->densities , GL_DYNAMIC_DRAW);
    
    glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(float), (void*)0);
    glEnableVertexAttribArray(3);
    glVertexAttribDivisor(3, 1);

    // PRESSURES
    glGenBuffers(1, &render_params->fluid_particles_pressures_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, render_params->fluid_particles_pressures_VBO);
    glBufferData(GL_ARRAY_BUFFER, sim_params->n_particles * sizeof(float), sim_params->pressures , GL_DYNAMIC_DRAW);
    
    glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, sizeof(float), (void*)0);
    glEnableVertexAttribArray(4);
    glVertexAttribDivisor(4, 1);
}

static void setup_bounding_buffers(fluid_render_paramters* render_params){
    // SIMULATION BOUNDINGS
    glGenBuffers(1, &render_params->fluid_sim_bounding_vertices_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, render_params->fluid_sim_bounding_vertices_VBO);
    glBufferData(GL_ARRAY_BUFFER, render_params->fluid_sim_bounding_n_vertices * 3 * sizeof(float), render_params->fluid_sim_bounding_vertices, GL_DYNAMIC_DRAW);

    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(2);

    // SIMULATION BOUNDING IBO
    glGenBuffers(1, &render_params->fluid_sim_bounding_IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, render_params->fluid_sim_bounding_IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, render_params->fluid_sim_bounding_n_indices*sizeof(unsigned), render_params->fluid_sim_bounding_indices, GL_DYNAMIC_DRAW);
}

static void setup_sim_grid_buffers(fluid_render_paramters* render_params, fluid_sim_parameters* sim_params){
    // grid cell vertices vbo
    glGenBuffers(1, &render_params->fluid_sim_grid_cell_vertices_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, render_params->fluid_sim_grid_cell_vertices_VBO);
    glBufferData(GL_ARRAY_BUFFER, render_params->fluid_sim_grid_cell_n_vertices * 3 * sizeof(float), render_params->fluid_sim_grid_cell_vertices, GL_DYNAMIC_DRAW);

    glVertexAttribPointer(11, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(11);

    // grid cell ibo
    glGenBuffers(1, &render_params->fluid_sim_grid_cell_IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, render_params->fluid_sim_grid_cell_IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, render_params->fluid_sim_grid_cell_n_indices*sizeof(unsigned), render_params->fluid_sim_grid_cell_indices, GL_DYNAMIC_DRAW);

    // whole grid
    glGenBuffers(1, &render_params->fluid_sim_grid_prefix_sums_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, render_params->fluid_sim_grid_prefix_sums_VBO);
    glBufferData(GL_ARRAY_BUFFER, sim_params->n_grid_cells_total * sizeof(int), sim_params->grid_cells_num_partciles_count , GL_DYNAMIC_DRAW);
    
    // gpu instancing
    glVertexAttribPointer(10, 1, GL_INT, GL_FALSE, sizeof(int), (void*)0);
    glEnableVertexAttribArray(10);
    glVertexAttribDivisor(10, 1);

}

static void setup_VAO(fluid_render_paramters* render_params){
    // VAO
    glGenVertexArrays(1, &render_params->VAO);
    glBindVertexArray(render_params->VAO);
}

void fluid_renderer_setup(fluid_render_paramters* render_params, fluid_sim_parameters* sim_params){
    // data
    setup_one_particle_data(render_params);
    setup_bounding_data(render_params, sim_params);
    setup_sim_grid_data(render_params, sim_params);
    
    // shaders
    setup_shaders(render_params);
    
    // buffers
    setup_VAO(render_params);
    setup_particles_buffers(render_params, sim_params);
    setup_fluid_data_buffers(render_params, sim_params);
    setup_bounding_buffers(render_params);
    setup_sim_grid_buffers(render_params, sim_params);
    
    
}

//                                                                 MAIN LOOP
// BOUNDING
static void loop_bounding_buffers(fluid_render_paramters* render_params){
    // SIMULATION BOUNDINGS
    
    glBindBuffer(GL_ARRAY_BUFFER, render_params->fluid_sim_bounding_vertices_VBO);
    glBufferData(GL_ARRAY_BUFFER, render_params->fluid_sim_bounding_n_vertices * 3 * sizeof(float), render_params->fluid_sim_bounding_vertices, GL_DYNAMIC_DRAW);

    // SIMULATION BOUNDINGS IBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, render_params->fluid_sim_bounding_IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, render_params->fluid_sim_bounding_n_indices*sizeof(unsigned), render_params->fluid_sim_bounding_indices, GL_DYNAMIC_DRAW);

}

void fluid_renderer_loop_draw_bounding(camera_3d* camera, fluid_render_paramters* render_params, fluid_sim_parameters* sim_params){
    //printf("baound_vertices[0]: %f\n", fluid_sim_bounding_vertices[0]);
    vec3 bound_pos;
    glm_vec3_copy(sim_params->bound_pos, bound_pos);

    // USE APPROPRIATE SHADER PROGRAM
    glUseProgram(render_params->bounding_shader_program);

    // BOUNDING BUFFER
    loop_bounding_buffers(render_params);

    // PASSING UNIFORMS
    int viewLoc = glGetUniformLocation(render_params->bounding_shader_program, "view");
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, camera->view[0]);
    int projectionLoc = glGetUniformLocation(render_params->bounding_shader_program, "projection");
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, camera->projection[0]);
    int boundingPosLoc = glGetUniformLocation(render_params->bounding_shader_program, "bounding_pos");
    glUniform3fv(boundingPosLoc, 1, bound_pos);

    //printf("sim bounding n indices: %d\n", fluid_sim_bounding_n_indices);
    //printf("sim bounding indices[35]: %d\n", fluid_sim_bounding_indices[35]);
    // DRAWING
    glBindVertexArray(render_params->VAO);
    glBindBuffer(GL_ARRAY_BUFFER, render_params->fluid_sim_bounding_vertices_VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, render_params->fluid_sim_bounding_IBO);
    glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
    glDrawElements(GL_TRIANGLES, render_params->fluid_sim_bounding_n_indices, GL_UNSIGNED_INT, 0);

}
// PARTICLES

static void loop_particles_buffers(fluid_render_paramters* render_params, fluid_sim_parameters* sim_params){
    vec3* fluid_particle_positions = sim_params->positions;

    // ONE FLUID PARTICLE
    glBindBuffer(GL_ARRAY_BUFFER, render_params->fluid_particle_vertices_VBO);
    glBufferData(GL_ARRAY_BUFFER, render_params->fluid_particle_n_vertices*3*sizeof(float), render_params->fluid_particle_vertices, GL_DYNAMIC_DRAW);
    
    // ALL FLUID PARTICLES
    glBindBuffer(GL_ARRAY_BUFFER, render_params->fluid_particles_pos_VBO);
    glBufferData(GL_ARRAY_BUFFER, sim_params->n_particles * 3 * sizeof(float), sim_params->positions , GL_DYNAMIC_DRAW);

    // ONE FLUID PARTICLE IBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, render_params->fluid_particle_IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, render_params->fluid_particle_n_indices*sizeof(unsigned), render_params->fluid_particle_indices, GL_DYNAMIC_DRAW);
}

static void loop_fluid_data_buffers(fluid_render_paramters* render_params, fluid_sim_parameters* sim_params){
    float* fluid_particle_densities = sim_params->densities;
    float* fluid_particle_pressures = sim_params->pressures;

    // DENSITIES
    glBindBuffer(GL_ARRAY_BUFFER, render_params->fluid_particles_densities_VBO);
    glBufferData(GL_ARRAY_BUFFER, sim_params->n_particles * sizeof(float), sim_params->densities , GL_DYNAMIC_DRAW);

    // PRESSURES
    glBindBuffer(GL_ARRAY_BUFFER, render_params->fluid_particles_pressures_VBO);
    glBufferData(GL_ARRAY_BUFFER, sim_params->n_particles * sizeof(float), sim_params->densities , GL_DYNAMIC_DRAW);

}

static void loop_fluid_buffers(fluid_render_paramters* render_params, fluid_sim_parameters* sim_params){
    loop_particles_buffers(render_params, sim_params);
    loop_fluid_data_buffers(render_params, sim_params);
}

void fluid_renderer_loop_draw_fluid_particles(camera_3d* camera, fluid_render_paramters* render_params, fluid_sim_parameters* sim_params){
    // USE APPROPRIATE SHADER PROGRAM
    glUseProgram(render_params->particle_shader_program);

    // FLUID PARTICLES BUFFER
    loop_fluid_buffers(render_params, sim_params);

    // PASSING UNIFORMS
    int viewLoc = glGetUniformLocation(render_params->particle_shader_program, "view");
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, camera->view[0]);
    int projectionLoc = glGetUniformLocation(render_params->particle_shader_program, "projection");
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, camera->projection[0]);
    int particleRadiusLoc = glGetUniformLocation(render_params->particle_shader_program, "particle_radius");
    glUniform1f(particleRadiusLoc, render_params->render_radius);

    // DRAWING
    glBindVertexArray(render_params->VAO);
    glBindBuffer(GL_ARRAY_BUFFER, render_params->fluid_particle_vertices_VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, render_params->fluid_particle_IBO);
    glBindBuffer(GL_ARRAY_BUFFER, render_params->fluid_particles_pos_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, render_params->fluid_particles_densities_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, render_params->fluid_particles_pressures_VBO);
    
    
    glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
    glDrawElementsInstanced(GL_TRIANGLES, render_params->fluid_particle_n_indices, GL_UNSIGNED_INT, 0, sim_params->n_particles);
}

// GRID

static void loop_sim_grid_cells_buffers(fluid_render_paramters* render_params, fluid_sim_parameters* sim_params){
    // grid cell vertices vbo
    glBindBuffer(GL_ARRAY_BUFFER, render_params->fluid_sim_grid_cell_vertices_VBO);
    glBufferData(GL_ARRAY_BUFFER, render_params->fluid_sim_grid_cell_n_vertices * 3 * sizeof(float), render_params->fluid_sim_grid_cell_vertices, GL_DYNAMIC_DRAW);

    // grid cell ibo
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, render_params->fluid_sim_grid_cell_IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, render_params->fluid_sim_grid_cell_n_indices*sizeof(unsigned), render_params->fluid_sim_grid_cell_indices, GL_DYNAMIC_DRAW);

    // whole grid
    glBindBuffer(GL_ARRAY_BUFFER, render_params->fluid_sim_grid_prefix_sums_VBO);
    glBufferData(GL_ARRAY_BUFFER, sim_params->n_grid_cells_total * sizeof(int), sim_params->grid_cells_num_partciles_count , GL_DYNAMIC_DRAW);
    
}

void fluid_renderer_loop_draw_sim_grid(camera_3d* camera, fluid_render_paramters* render_params, fluid_sim_parameters* sim_params){
    // USE APPROPRIATE SHADER PROGRAM
    glUseProgram(render_params->grid_shader_program);

    // UPDATE BUFFERS
    loop_sim_grid_cells_buffers(render_params, sim_params);

    // PASSING UNIFORMS
    int viewLoc = glGetUniformLocation(render_params->grid_shader_program, "view");
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, camera->view[0]);
    
    int projectionLoc = glGetUniformLocation(render_params->grid_shader_program, "projection");
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, camera->projection[0]);

    int boundingPosLoc = glGetUniformLocation(render_params->grid_shader_program, "bounding_pos");
    glUniform3fv(boundingPosLoc, 1, sim_params->bound_pos);

    int boundingDimsLoc = glGetUniformLocation(render_params->grid_shader_program, "bounding_dims");
    glUniform3fv(boundingDimsLoc, 1, sim_params->bound_dims);
    
    int cellDimsLoc = glGetUniformLocation(render_params->grid_shader_program, "cell_dims");
    glUniform3fv(cellDimsLoc, 1, render_params->fluid_sim_grid_cell_dims);
    
    int nCellsX = glGetUniformLocation(render_params->grid_shader_program, "n_cells_x");
    glUniform1i(nCellsX, sim_params->n_grid_cells_x);
    int nCellsY = glGetUniformLocation(render_params->grid_shader_program, "n_cells_y");
    glUniform1i(nCellsY, sim_params->n_grid_cells_y);
    int nCellsZ = glGetUniformLocation(render_params->grid_shader_program, "n_cells_z");
    glUniform1i(nCellsZ, sim_params->n_grid_cells_z);
    

    // DRAWING
    // whole grid
    glBindBuffer(GL_ARRAY_BUFFER, render_params->fluid_sim_grid_prefix_sums_VBO);
    // grid cell vertices vbo
    glBindBuffer(GL_ARRAY_BUFFER, render_params->fluid_sim_grid_cell_vertices_VBO);
    // grid cell ibo
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, render_params->fluid_sim_grid_cell_IBO);
    

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glDrawElementsInstanced(GL_TRIANGLES, render_params->fluid_sim_grid_cell_n_indices, GL_UNSIGNED_INT, 0, sim_params->n_grid_cells_total);
    //glDrawElementsInstanced(GL_TRIANGLES, render_params->)
}

