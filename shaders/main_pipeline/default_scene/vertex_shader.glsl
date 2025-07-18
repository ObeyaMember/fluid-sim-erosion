#version 460 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in vec3 instPos;

out vec2 TexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

//uniform vec3 bPos[5];

void main()
{
//gl_Position = projection * view * vec4(aPos + bPos[gl_InstanceID], 1.0);
gl_Position = projection * view * vec4(aPos + instPos, 1.0);

//gl_Position = vec4(aPos.x, aPos.y, 0.1, 1.0);
TexCoord = aTexCoord;
}