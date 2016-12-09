#version 120

uniform mat4 perspective;
uniform mat4 view;
uniform vec4 eye;
uniform vec4 light_position;

attribute vec4 vNorm;
attribute vec4 vPos;

varying vec4 color;
varying vec4 f_vNorm;
varying vec4 f_light_direction;
varying vec4 f_view_vec;
varying vec4 pos;

void main()
{
    vec4 light_direction = normalize(light_position - vPos);
    vec4 view_vec = normalize(eye - vPos);

    gl_Position = perspective * view * vPos;
    pos = vPos;
    f_vNorm = vNorm;
    f_light_direction = light_direction;
    f_view_vec = view_vec;
}

