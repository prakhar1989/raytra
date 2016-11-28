#version 120

uniform mat4 MVP;

// same as in..
attribute vec4 vCol;
attribute vec4 vPos;

// same as out..
varying vec4 color;

void main()
{
    gl_Position = MVP * vPos;
    color = vCol;
}
