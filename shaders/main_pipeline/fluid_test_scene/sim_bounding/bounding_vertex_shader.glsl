#version 460 core

/* layout (location = 0) in vec3 particle_plane_ver_pos;
layout (location = 1) in vec3 particle_pos; */
layout (location = 2) in vec3 bounding_ver_pos;


uniform mat4 view;
uniform mat4 projection;

//uniform vec3 bPos[5];

void main()
{

gl_Position = projection * view * vec4(bounding_ver_pos, 1.0);



}