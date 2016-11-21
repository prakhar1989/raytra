#version 120

// the simplest fragment shader: get the color (from the vertex shader -
// actually interpolated from values specified in the vertex shader - and
// just pass them through to the render:
// 
// on the mac, you may need to say "varying vec4 color;" instead of this:
//varying vec4 color;


// in GLSL 150, use layouts for this:
// layout (location = 0) in color

// in later GLSL, use "in", and "out" respectively

// for transform:
//varying vec3 color;

// for lit:
varying vec4 color;

void main() 
{ 
  // "gl_FragColor" is already defined for us - it's the one thing you have
  // to set in the fragment shader:

//    gl_FragColor = vec4(color, 1.0);
    gl_FragColor = color;
}

