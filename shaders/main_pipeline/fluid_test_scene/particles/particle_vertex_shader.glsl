#version 460 core

layout (location = 0) in vec3 particle_plane_ver_pos;
layout (location = 1) in vec3 particle_pos;



uniform mat4 view;
uniform mat4 projection;


void main()
{

gl_Position = projection * view * vec4(particle_plane_ver_pos + particle_pos, 1.0);



}