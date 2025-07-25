#version 460 core

out vec4 FragColor;

in float ParticleRadius;
in float ParticleDensity;
in float ParticlePressure;

void main()
{

float to_show = ParticleDensity;
float to_show_target = 0.05;
float to_show_dev = to_show_target / 10.0;
float to_show_max = to_show_target * 2.0;

vec4 color = vec4(0,0,0,1);
if (to_show <= to_show_target + to_show_dev && to_show >= to_show_target - to_show_dev){
    color = vec4(0.337, 0.878, 0.369, 1);
}
else if (to_show <= to_show_max){
    float to_show_ratio = to_show / to_show_max;
    color = vec4(to_show_ratio, to_show_ratio, to_show_ratio, 1);
}else {
    color = vec4(1,0,0,1);
}

//FragColor = color;
FragColor = vec4(0.118, 0.518, 0.969, 1); // blue

}