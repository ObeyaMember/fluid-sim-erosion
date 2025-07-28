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

void camera_3d_angles_update(GLFWwindow* window, vec3 camera_dir, float camera_look_speed, float* camera_pitch, float* camera_yaw, float* last_mouse_x, float* last_mouse_y, float delta_time){
    
    double new_mouse_x, new_mouse_y, x_offset, y_offset;
    glfwGetCursorPos(window, &new_mouse_x, &new_mouse_y);
    
    x_offset = (new_mouse_x - *last_mouse_x); // WHY TF DO THESE RANDOMLY NEED TO BE MADE OPPOSITE
    y_offset = (new_mouse_y - *last_mouse_y);

    *last_mouse_x = new_mouse_x;
    *last_mouse_y = new_mouse_y;
    
    *camera_yaw = *camera_yaw + (x_offset * camera_look_speed * delta_time);
    *camera_pitch = glm_clamp(*camera_pitch + (y_offset * camera_look_speed * delta_time), 91, 269);
}

void camera_3d_direction_update(GLFWwindow* window, vec3 camera_pos, vec3 camera_dir, mat4 view, float camera_pitch, float camera_yaw){
    
    camera_dir[0] = cos(glm_rad(camera_yaw)) * cos(glm_rad(camera_pitch));
    camera_dir[1] = sin(glm_rad(camera_pitch));
    camera_dir[2] = sin(glm_rad(camera_yaw)) * cos(glm_rad(camera_pitch));

    glm_vec3_normalize(camera_dir);
    vec3 target;
    glm_vec3_add(camera_pos, camera_dir, target);
    glm_lookat(camera_pos, target, c_y_axis, view);

}

void camera_3d_move_update(GLFWwindow* window, vec3 camera_pos, vec3 camera_dir, mat4 view, float camera_move_speed, float delta_time){
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS){
        vec3 a;
        glm_vec3_scale(camera_dir, camera_move_speed * delta_time, a); // WHY TF DO THESE RANDOMLY NEED TO BE MADE OPPOSITE
        glm_vec3_add(camera_pos, a, camera_pos);
        //printf("PRESSED:  W\n");
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS){
        vec3 a;
        glm_vec3_scale(camera_dir, -camera_move_speed * delta_time, a);
        glm_vec3_add(camera_pos, a, camera_pos);
        //printf("PRESSED:  S\n");
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS){
        vec3 a, right;
        glm_vec3_cross(camera_dir, c_y_axis, right);
        glm_vec3_normalize(right);
        glm_vec3_scale(right, camera_move_speed * delta_time, a);
        glm_vec3_add(camera_pos, a, camera_pos);
        //printf("PRESSED:  D\n");
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS){
        vec3 a, right;
        glm_vec3_cross(camera_dir, c_y_axis, right);
        glm_vec3_normalize(right);
        glm_vec3_scale(right, -camera_move_speed * delta_time, a);
        glm_vec3_add(camera_pos, a, camera_pos);
        //printf("PRESSED:  A\n");
    }
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS){
        vec3 a;
        glm_vec3_scale(c_y_axis, camera_move_speed * delta_time, a);
        glm_vec3_add(camera_pos, a, camera_pos);
        //printf("PRESSED:  SPACE\n");
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS){
        vec3 a;
        glm_vec3_scale(c_y_axis, -camera_move_speed * delta_time, a);
        glm_vec3_add(camera_pos, a, camera_pos);
        //printf("PRESSED:  LEFT_SHIFT\n");
    }

    glm_translate(view, camera_pos);

    //printf("x, y, z: %f, %f, %f\n", camera_pos[0], camera_pos[1], camera_pos[2]);
}

void camera_3d_zoom_update(GLFWwindow* window, float* camera_fov, float camera_zoom_speed){
    
}



