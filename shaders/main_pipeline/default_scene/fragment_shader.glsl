#version 460 core

out vec4 FragColor;
in vec2 TexCoord;
uniform sampler2D texture1;
void main()
{
FragColor = texture(texture1, TexCoord);

//FragColor = vec4(0.988,0.596,0.231,1);
}