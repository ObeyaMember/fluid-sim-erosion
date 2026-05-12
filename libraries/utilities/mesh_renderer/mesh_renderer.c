#include "mesh_renderer.h"

// SHADERS
static void setup_mesh_shaders(mesh_renderer* m_renderer){
    // shaders
    const char* mesh_vertex_shader_source = get_shader_content("./shaders/main_pipeline/mesh/mesh_vertex_shader.glsl");
    const char* mesh_fragment_shader_source = get_shader_content("./shaders/main_pipeline/mesh/mesh_fragment_shader.glsl");
    
    setup_fvc_shader(&m_renderer->mesh_vertex_shader, &mesh_vertex_shader_source, "vertex");
    fvc_shader_comp_error(&m_renderer->mesh_vertex_shader, "vertex");
    
    setup_fvc_shader(&m_renderer->mesh_fragment_shader, &mesh_fragment_shader_source, "fragment");
    fvc_shader_comp_error(&m_renderer->mesh_fragment_shader, "fragment");

    // shader programs
    setup_fvc_shader_prog(&m_renderer->mesh_shader_program, &m_renderer->mesh_fragment_shader, &m_renderer->mesh_vertex_shader, NULL, "main_pipeline");
    fvc_shader_prog_link_error(&m_renderer->mesh_shader_program, "main_pipeline");
}

// BUFFERS
static void setup_mesh_buffers(mesh_renderer* m_renderer, mesh* m){
    glGenBuffers(1, &m_renderer->mesh_vertices_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, m_renderer->mesh_vertices_VBO);
    glBufferData(GL_ARRAY_BUFFER, m->mesh_n_vertices*3*sizeof(float), m->mesh_vertices, GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glGenBuffers(1, &m_renderer->mesh_IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_renderer->mesh_IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m->mesh_n_indices*sizeof(unsigned), m->mesh_indices, GL_DYNAMIC_DRAW);
}

static void setup_mesh_VAO(mesh_renderer* m_renderer){
    glGenVertexArrays(1, &m_renderer->VAO);
    glBindVertexArray(m_renderer->VAO);
}

// FINAL SETUP
void mesh_renderer_setup(mesh_renderer* m_renderer, mesh* m){
    setup_mesh_shaders(m_renderer);
    setup_mesh_VAO(m_renderer);
    setup_mesh_buffers(m_renderer, m);
}   

// MAIN LOOP
// loop buffers
static void loop_mesh_buffers(mesh_renderer* m_renderer, mesh* m){
    glBindBuffer(GL_ARRAY_BUFFER, m_renderer->mesh_vertices_VBO);
    glBufferData(GL_ARRAY_BUFFER, m->mesh_n_vertices*3*sizeof(float), m->mesh_vertices, GL_DYNAMIC_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_renderer->mesh_IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m->mesh_n_indices*sizeof(unsigned), m->mesh_indices, GL_DYNAMIC_DRAW);
    
}
// loop draw
void mesh_renderer_loop_draw_mesh(camera_3d* camera, mesh_renderer* m_renderer, mesh* m){
    vec3 mesh_pos;
    glm_vec3_copy(m->pos, mesh_pos);
    
    // use shader program
    glUseProgram(m_renderer->mesh_shader_program);

    // mesh buffers
    loop_mesh_buffers(m_renderer, m);

    // passing uniforms
    int viewLoc = glGetUniformLocation(m_renderer->mesh_shader_program, "view");
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, camera->view[0]);
    int projectionLoc = glGetUniformLocation(m_renderer->mesh_shader_program, "projection");
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, camera->projection[0]);
    int boundingPosLoc = glGetUniformLocation(m_renderer->mesh_shader_program, "mesh_pos");
    glUniform3fv(boundingPosLoc, 1, mesh_pos);
    
    // drawing
    glBindVertexArray(m_renderer->VAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_renderer->mesh_vertices_VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_renderer->mesh_IBO);
    glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
    glDrawElements(GL_TRIANGLES, m->mesh_n_indices, GL_UNSIGNED_INT, 0);

}

// END
void mesh_renderer_end(mesh_renderer* m_renderer){

}