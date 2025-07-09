#include "camera.h"

vec3 c_x_axis = {1,0,0};
vec3 c_y_axis = {0,1,0};
vec3 c_z_axis = {0,0,1};

/*

camera features:
    - move around with arrows or wasd
    - drag around with mouse (?)
    - zoom with wheel
    - set to orthographic or perspective (ui? -> yes - > imgui)
*/

void camera_3d_direction_update(GLFWwindow* window, vec3 camera_dir, float camera_pitch, float camera_yaw){

}

void camera_3d_move_update(GLFWwindow* window, vec3 camera_pos, vec3 camera_dir, float camera_move_speed, float delta_time){
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS){
        vec3 a;
        glm_vec3_scale(c_z_axis, camera_move_speed * delta_time, a);
        glm_vec3_add(camera_pos, a, camera_pos);
        //printf("PRESSED:  W\n");
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS){
        vec3 a;
        glm_vec3_scale(c_z_axis, -camera_move_speed * delta_time, a);
        glm_vec3_add(camera_pos, a, camera_pos);
        //printf("PRESSED:  S\n");
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS){
        vec3 a;
        glm_vec3_scale(c_x_axis, -camera_move_speed * delta_time, a);
        glm_vec3_add(camera_pos, a, camera_pos);
        //printf("PRESSED:  D\n");
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS){
        vec3 a;
        glm_vec3_scale(c_x_axis, camera_move_speed * delta_time, a);
        glm_vec3_add(camera_pos, a, camera_pos);
        //printf("PRESSED:  A\n");
    }
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS){
        vec3 a;
        glm_vec3_scale(c_y_axis, -camera_move_speed * delta_time, a);
        glm_vec3_add(camera_pos, a, camera_pos);
        //printf("PRESSED:  SPACE\n");
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS){
        vec3 a;
        glm_vec3_scale(c_y_axis, camera_move_speed * delta_time, a);
        glm_vec3_add(camera_pos, a, camera_pos);
        //printf("PRESSED:  LEFT_SHIFT\n");
    }

    //printf("x, y, z: %f, %f, %f\n", camera_pos[0], camera_pos[1], camera_pos[2]);
}

void camera_3d_zoom_update(GLFWwindow* window, float* camera_fov, float camera_zoom_speed){

}



