#ifndef _MESH_RENDERER_H_
#define _MESH_RENDERER_H_

#include "../../default_libs/default_libs.h"
#include "../../glad/glad.h"
#include "../../cglm/include/cglm/cglm.h"
#include "../../stb_image/stb_image.h"
#include "../../GLFW/glfw3.h"
#include "../heightmap/heightmap.h"
#include "../mesh/mesh.h"
#include "../camera/camera.h"
#include "../shaders/shaders.h"



// TO CHANGE NAMES OF EVERYTHING    !!!!!!! 
// TO CHANGE NAMES OF EVERYTHING    !!!!!!!
// TO CHANGE NAMES OF EVERYTHING    !!!!!!!
// TO CHANGE NAMES OF EVERYTHING    !!!!!!!

struct mesh_renderer_s{
    //                                                  SHADERS AND SHADER PROGRAMS
    unsigned int mesh_vertex_shader;
    unsigned int mesh_fragment_shader;
    unsigned int mesh_shader_program;

    //                                                              BUFFERS
    // vao
    unsigned int VAO;

    unsigned int mesh_vertices_VBO;
    unsigned int mesh_IBO;
};
typedef struct mesh_renderer_s mesh_renderer;

void mesh_renderer_setup(mesh_renderer* m_renderer, mesh* m);
void mesh_renderer_loop_draw_mesh(camera_3d* camera, mesh_renderer* m_renderer, mesh* m);
void mesh_renderer_end(mesh_renderer* m_renderer);

#endif