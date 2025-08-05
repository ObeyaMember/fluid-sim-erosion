#version 460 core

out vec4 FragColor;

in flat int GridCellNumParticlesCount;

void main()
{

vec3 turqoise = vec3(0.369, 0.596, 0.71);

//FragColor = texture(texture1, TexCoord);
if (GridCellNumParticlesCount == 0){
    FragColor = vec4(0, 0, 0, 0);
}else {
    FragColor = vec4(turqoise, 1);
}

}