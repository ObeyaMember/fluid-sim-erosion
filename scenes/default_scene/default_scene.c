#include "default_scene.h"
//                                  SCENE VARIABLES
//                                      WORLD
vec3 x_axis = {1,0,0};
vec3 y_axis = {0,1,0};
vec3 z_axis = {0,0,1};
mat4 model, view, projection;

//                                      CAMERA
float camera_fov = 100.0;
float camera_aspect = 1;
float camera_near = 0.01;
float camera_far = 1000.0;
float camera_pitch = 0.0;
float camera_yaw = -90.0;
float camera_move_speed = 1;
float camera_look_speed = 20;
float camera_zoom_speed = 1;
vec3 camera_pos;
vec3 camera_dir;


//                                      ENGINE 
float delta_time = 1.0;
float last_frame_time = 0.0;
float mouse_x = 400;
float mouse_y = 400;

//                                      OBJECTS
//                                      PLANE
int plane_num_ver = 4;
float plane_vertices[] = {
    // positions      // texture coords
    0.5f, 0.5f, 0.0, // top right
    0.5f, -0.5f, 0.0,// bottom right
    -0.5f, -0.5f, 0.0,// bottom left
    -0.5f, 0.5f, 0.0,// top left
};

float* plane_ver;
float* plane_tex_c;

int plane_num_indices = 6;
unsigned int plane_indices[] = { // note that we start from 0!
        0, 1, 3, // first triangle
        1, 2, 3 // second triangle
};

float plane_text_coords[] = {
    1.0, 1.0,
    1.0, 0.0,
    0.0, 0.0,
    0.0, 1.0
};



//                                      CUBE 1
float* cube_1_vertices;
float* cube_1_tex_coords; 
unsigned int* cube_1_indices;
float cube_1_side_size = 1;
int cube_1_n_vertices;
int cube_1_n_indices;

//                                  CHANGING OBEJCT 
float* chng_obj_vertices;
float* chng_obj_tex_coords; 
unsigned int* chng_obj_indices;
float chng_obj_side_size = 1;
int chng_obj_n_vertices;
int chng_obj_n_indices;




//                                  SHADERS AND SHADER PROGRAMS
unsigned int vertex_shader, fragment_shader;
unsigned int main_shader_program;

//                                      BUFFERS / TEXTURES

unsigned int posVBO, texVBO, VAO, IBO;
unsigned int plane_texture;


void default_scene_setup(GLFWwindow* window){

    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // SHADERS
    const char* vertex_shader_source = get_shader_content("./shaders/main_pipeline/default_scene/vertex_shader.glsl");

    const char* fragment_shader_source = get_shader_content("./shaders/main_pipeline/default_scene/fragment_shader.glsl");
    
    
    setup_fvc_shader(&vertex_shader, &vertex_shader_source, "vertex");
    fvc_shader_comp_error(&vertex_shader, "vertex");
    
    setup_fvc_shader(&fragment_shader, &fragment_shader_source, "fragment");
    fvc_shader_comp_error(&fragment_shader, "fragment");

    // SHADER PROGRAMS
    setup_fvc_shader_prog(&main_shader_program, &fragment_shader, &vertex_shader, NULL, "main_pipeline");
    fvc_shader_prog_link_error(&main_shader_program, "main_pipeline");
    
    // DATA
    // OBJECTS
    // CUBE 1
    //cube_1_vertices = get_prim_cube_w_tex_coords(cube_1_indices, cube_1_tex_coords, &cube_1_n_vertices, &cube_1_n_indices, cube_1_side_size);
    
    // CHANGING OBJECT
    chng_obj_vertices = malloc(8*3*sizeof(float));
    chng_obj_indices = malloc(36*sizeof(unsigned));
    chng_obj_tex_coords= malloc(8*2*sizeof(float));
    get_prim_cube_w_tex_coords(chng_obj_vertices, chng_obj_indices, chng_obj_tex_coords, &chng_obj_n_vertices, &chng_obj_n_indices, chng_obj_side_size);
    printf("N VERTICES %d\n", chng_obj_n_vertices);
    printf("N INDICES %d\n", chng_obj_n_indices);
    
    // TEXTURES
    
    setup_texture_from_png(&plane_texture, "./assets/textures/awesome_face.png", &main_shader_program, "texture1", 0);

    // BUFFERS
    // PLANE

    plane_ver = malloc(plane_num_ver*3*sizeof(float));
    for (int i = 0; i < 3 * plane_num_ver; i += 1){
        plane_ver[i] = plane_vertices[i];
    }

    plane_tex_c = malloc(plane_num_ver*2*sizeof(float));
    for (int i = 0; i < 2 * plane_num_ver; i += 1){
        plane_tex_c[i] = plane_text_coords[i];
    }

   /*  glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    
    glGenBuffers(1, &posVBO);
    glBindBuffer(GL_ARRAY_BUFFER, posVBO);
    glBufferData(GL_ARRAY_BUFFER, plane_num_ver*3*sizeof(float), plane_ver, GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    glGenBuffers(1, &texVBO);
    glBindBuffer(GL_ARRAY_BUFFER, texVBO);
    glBufferData(GL_ARRAY_BUFFER, plane_num_ver*2*sizeof(float), plane_tex_c, GL_DYNAMIC_DRAW);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);

    glGenBuffers(1, &IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, plane_num_indices*sizeof(unsigned), plane_indices, GL_DYNAMIC_DRAW); */

    // CUBE1
    /* glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    
    glGenBuffers(1, &posVBO);
    glBindBuffer(GL_ARRAY_BUFFER, posVBO);
    glBufferData(GL_ARRAY_BUFFER, cube_1_n_vertices*3*sizeof(float), cube_1_vertices, GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    glGenBuffers(1, &texVBO);
    glBindBuffer(GL_ARRAY_BUFFER, texVBO);
    glBufferData(GL_ARRAY_BUFFER, cube_1_n_vertices*2*sizeof(float), cube_1_tex_coords, GL_DYNAMIC_DRAW);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);

    glGenBuffers(1, &IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, cube_1_n_indices*sizeof(unsigned), cube_1_indices, GL_DYNAMIC_DRAW); */

    for (int i = 0; i < chng_obj_n_vertices*3; i += 1){
        printf("%f\n", chng_obj_vertices[i]);
    }

    // CHANGING OBJECT
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    
    glGenBuffers(1, &posVBO);
    glBindBuffer(GL_ARRAY_BUFFER, posVBO);
    glBufferData(GL_ARRAY_BUFFER, chng_obj_n_vertices*3*sizeof(float), chng_obj_vertices, GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    glGenBuffers(1, &texVBO);
    glBindBuffer(GL_ARRAY_BUFFER, texVBO);
    glBufferData(GL_ARRAY_BUFFER, chng_obj_n_vertices*2*sizeof(float), chng_obj_tex_coords, GL_DYNAMIC_DRAW);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);

    glGenBuffers(1, &IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, chng_obj_n_indices*sizeof(unsigned), chng_obj_indices, GL_DYNAMIC_DRAW);


    // WORLD
    vec3 a;
    glm_vec3_scale(z_axis, -1, a);
    glm_mat4_identity(model);
    glm_translate(model, a);
    glm_rotate(model, glm_rad(0), x_axis);
    glm_mat4_identity(view);
    
    
    glm_mat4_identity(view);
    glm_vec3_copy(a, camera_pos);
    glm_translate(view, camera_pos);

    glm_perspective(glm_rad(camera_fov), camera_aspect, camera_near, camera_far, projection);

    // CAMERA
    glm_vec3_zero(camera_pos);
    glm_vec3_zero(camera_dir);
/*     print_yes();
    print_yes_shaders();
    print_lmao(); */
    

}

void default_scene_main_loop(GLFWwindow* window){

    // DELTA TIME
    float new_frame_time = glfwGetTime();
    delta_time = new_frame_time - last_frame_time;
    last_frame_time = new_frame_time;

    glEnable(GL_DEPTH_TEST);
    glClearColor(0.3f, 0.4f, 0.6f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    

    glUseProgram(main_shader_program);

    // UPDATE THE TRANSFORM MATRICES
    glm_rotate(model, glm_rad(0.3), x_axis);
    
    glm_mat4_identity(view); // RESET VIEW MATRIX AFTER EACH FRAME BEFORE UPDATING CAMERA
    camera_3d_angles_update(window, camera_dir, camera_look_speed, &camera_pitch, &camera_yaw, &mouse_x, &mouse_y, delta_time);
    //printf("pitch: %f, yaw: %f\n", camera_pitch, camera_yaw);
    camera_3d_move_update(window, camera_pos, camera_dir, view, camera_move_speed, delta_time);
    camera_3d_direction_update(window, camera_pos, camera_dir, view, camera_pitch, camera_yaw);
    


    // PASSING THE TRANSFORM MATRICES
    int modelLoc = glGetUniformLocation(main_shader_program, "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, model[0]);
    int viewLoc = glGetUniformLocation(main_shader_program, "view");
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, view[0]);
    int projectionLoc = glGetUniformLocation(main_shader_program, "projection");
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, projection[0]);

    // UPDATE BUFFERS
    // PLANE
    /* glBindBuffer(GL_ARRAY_BUFFER, posVBO);
    glBufferData(GL_ARRAY_BUFFER, plane_num_ver*3*sizeof(float), plane_ver, GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, texVBO);
    glBufferData(GL_ARRAY_BUFFER, plane_num_ver*2*sizeof(float), plane_tex_c, GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, plane_num_indices*sizeof(unsigned), plane_indices, GL_DYNAMIC_DRAW); */

    // CUBE
    /* glBindBuffer(GL_ARRAY_BUFFER, posVBO);
    glBufferData(GL_ARRAY_BUFFER, cube_1_n_vertices*3*sizeof(float), cube_1_vertices, GL_DYNAMIC_DRAW);
    
    glBindBuffer(GL_ARRAY_BUFFER, texVBO);
    glBufferData(GL_ARRAY_BUFFER, cube_1_n_vertices*2*sizeof(float), cube_1_tex_coords, GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, cube_1_n_indices*sizeof(unsigned), cube_1_indices, GL_DYNAMIC_DRAW); */

    // CHANGING OBJECT

    glBindBuffer(GL_ARRAY_BUFFER, posVBO);
    glBufferData(GL_ARRAY_BUFFER, chng_obj_n_vertices*3*sizeof(float), chng_obj_vertices, GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, texVBO);
    glBufferData(GL_ARRAY_BUFFER, chng_obj_n_vertices*2*sizeof(float), chng_obj_tex_coords, GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, chng_obj_n_indices*sizeof(unsigned), chng_obj_indices, GL_DYNAMIC_DRAW);


    // PASSING THE TEXTURES
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, plane_texture);

    glUniform1i(glGetUniformLocation(main_shader_program, "texture1"), 0);

    glBindVertexArray(VAO);
    
    
    glPointSize(5);
    //glDrawArrays(GL_POINTS, 0, 8);
    
    //glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
    glDrawElements(GL_TRIANGLES, chng_obj_n_indices, GL_UNSIGNED_INT, 0);

}

void default_scene_end(GLFWwindow* window){
    free_prim_cube_w_tex_coords(cube_1_vertices, cube_1_indices, cube_1_tex_coords);
    free_prim_plane_w_tex_coords(chng_obj_vertices, chng_obj_indices, chng_obj_tex_coords);
}