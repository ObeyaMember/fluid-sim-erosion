#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "../../default_libs/default_libs.h"
#include "../../glad/glad.h"
#include "../../cglm/include/cglm/cglm.h"
#include "../../stb_image/stb_image.h"
#include "../../GLFW/glfw3.h"

struct camera_3d_s {
    float camera_fov;
    float camera_aspect;
    float camera_near;
    float camera_far;
    float camera_pitch;
    float camera_yaw;
    float camera_move_speed;
    float camera_look_speed;
    float camera_zoom_speed;
    vec3 camera_pos;
    vec3 camera_dir;

    mat4 view;
    mat4 projection;

};

typedef struct camera_3d_s camera_3d;

//void camera_3d_angles_update(GLFWwindow* window, vec3 camera_dir, float camera_look_speed, float* camera_pitch, float* camera_yaw, float* last_mouse_x, float* last_mouse_y, float delta_time);
//void camera_3d_direction_update(GLFWwindow* window, vec3 camera_pos, vec3 camera_dir, mat4 view, float camera_pitch, float camera_yaw);
//void camera_3d_move_update(GLFWwindow* window, vec3 camera_pos, vec3 camera_dir, mat4 view, float camera_move_speed, float delta_time);
//void camera_3d_zoom_update(GLFWwindow* window, float* camera_fov, float camera_zoom_speed);

void camera_setup(camera_3d* camera);
void camera_loop(GLFWwindow* window, camera_3d* camera, float* mouse_x, float* mouse_y, float delta_time);

#endif