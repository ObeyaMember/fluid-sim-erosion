#version 460 core

layout (location = 0) in vec3 particle_plane_ver_pos;
layout (location = 1) in vec3 particle_pos;
layout (location = 3) in float particle_density;
layout (location = 4) in float particle_pressure;


uniform mat4 view;
uniform mat4 projection;

out float ParticleRadius;
out float ParticleDensity;
out float ParticlePressure;

void main()
{

gl_Position = projection * view * vec4(particle_plane_ver_pos + particle_pos, 1.0);
ParticleDensity = particle_density;


}