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
float cam_pitch = 0.0;
float cam_yaw = -90.0;
float camera_move_speed = 1;
float camera_look_speed = 1;
float camera_zoom_speed = 1;
vec3 camera_pos;
vec3 camera_dir;


//                                      ENGINE 
float delta_time = 1.0;
float last_frame_time = 0.0;

//                                      OBJECTS
int plane_num_ver = 4;
float plane_vertices[] = {
    // positions      // texture coords
    0.5f, 0.5f, 0.0, // top right
    0.5f, -0.5f, 0.0,// bottom right
    -0.5f, -0.5f, 0.0,// bottom left
    -0.5f, 0.5f, 0.0,// top left
};

float plane_text_coords[] = {
    1.0, 1.0,
    1.0, 0.0,
    0.0, 0.0,
    0.0, 1.0
};


int plane_num_indices = 6;
unsigned int plane_indices[] = { // note that we start from 0!
        0, 1, 3, // first triangle
        1, 2, 3 // second triangle
};



//                                  SHADERS AND SHADER PROGRAMS
unsigned int vertex_shader, fragment_shader;
unsigned int main_shader_program;

//                                      BUFFERS / TEXTURES

unsigned int posVBO, texVBO, VAO, IBO;
unsigned int plane_texture;


void default_scene_setup(GLFWwindow* window){

    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

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
    // TEXTURES
    
    setup_texture_from_png(&plane_texture, "./assets/textures/awesome_face.png", &main_shader_program, "texture1", 0);

    // BUFFERS
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    
    glGenBuffers(1, &posVBO);
    glBindBuffer(GL_ARRAY_BUFFER, posVBO);
    glBufferData(GL_ARRAY_BUFFER, plane_num_ver*3*sizeof(float), plane_vertices, GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    glGenBuffers(1, &texVBO);
    glBindBuffer(GL_ARRAY_BUFFER, texVBO);
    glBufferData(GL_ARRAY_BUFFER, plane_num_ver*2*sizeof(float), plane_text_coords, GL_DYNAMIC_DRAW);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);

    glGenBuffers(1, &IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, plane_num_indices*sizeof(unsigned), plane_indices, GL_DYNAMIC_DRAW);

    // WORLD
    glm_mat4_identity(model);
    glm_rotate(model, glm_rad(0), x_axis);
    glm_mat4_identity(view);
    
    vec3 a;
    glm_vec3_scale(z_axis, -1, a);
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
    glm_mat4_identity(view);
    glm_translate(view, camera_pos);

    camera_3d_move_update(window, camera_pos, camera_dir, camera_move_speed, delta_time);


    // PASSING THE TRANSFORM MATRICES
    int modelLoc = glGetUniformLocation(main_shader_program, "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, model[0]);
    int viewLoc = glGetUniformLocation(main_shader_program, "view");
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, view[0]);
    int projectionLoc = glGetUniformLocation(main_shader_program, "projection");
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, projection[0]);

    // UPDATE BUFFERS

    glBindBuffer(GL_ARRAY_BUFFER, posVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(plane_vertices), plane_vertices, GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, texVBO);
    glBufferData(GL_ARRAY_BUFFER, plane_num_ver*2*sizeof(float), plane_text_coords, GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, plane_num_indices*sizeof(unsigned), plane_indices, GL_DYNAMIC_DRAW);

    // PASSING THE TEXTURES
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, plane_texture);

    glUniform1i(glGetUniformLocation(main_shader_program, "texture1"), 0);

    glBindVertexArray(VAO);
    glPointSize(5);
    //glDrawArrays(GL_POINTS, 0, 5);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

}   