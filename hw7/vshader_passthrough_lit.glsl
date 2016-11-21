#version 120

#ifdef NEVER
// a contrived vertex shader, that colors each vertex in a triangle
// according to its position

// we are going to be getting an attribute from the main program, named
// "vPosition", one for each vertex.
//
// on mac you may have to say: "attribute vec4 vPosition;" instead of "in...":
attribute vec4 vPosition;

// we are also going to be getting color values, per-vertex, from the main
// program running on the cpu (and that are then stored in the VBO on the
// card. this color, called "vColor", is just going to be passed through 
// to the fragment shader, which will intrpolate its value per-fragment
// amongst the 3 vertex colors on the triangle that fragment is a part of.
//
// on mac you may have to say: "attribute vec4 vColor;" instead of this:
attribute vec4 vColor;

// we are going to be outputting a single 4-vector, called color, which
// may be different for each vertex.
// the fragment shader will be expecting these values, and interpolate
// them according to the distance of the fragment from the vertices
//
// on mac you may have to say: "varying vec4 color;" instead of "out ...":
varying vec4 color;

void main() 
{
  gl_Position = vPosition;
  color = vColor;
} 
#endif

uniform mat4 MVP;

// in later GLSL, use "in", and "out" respectively

// for transform:
//attribute vec3 vCol;
//attribute vec2 vPos;
//varying vec3 color;

// for lit:
attribute vec4 vCol;
attribute vec4 vPos;
varying vec4 color;

void main()
{
 //   gl_Position = MVP * vec4(vPos, 0.0, 1.0);
    gl_Position = MVP * vPos;
    color = vCol;
}


