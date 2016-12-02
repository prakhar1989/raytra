#version 120

uniform mat4 MVP;

// camera location
uniform vec4 eye;

// light properties
uniform vec4 light_diffuse;
uniform vec4 light_specular;
uniform vec4 light_ambient;
uniform vec4 light_position;

// material properties
uniform vec4 material_diffuse;
uniform vec4 material_specular;
uniform vec4 material_ambient;
uniform float material_shininess;

// same as in..
attribute vec4 vNorm;
attribute vec4 vPos;

// same as out..
varying vec4 color;

void main()
{
    vec4 ambient_color = material_ambient * light_ambient;

    vec4 light_direction = normalize(light_position - vPos);
    float dd1 = max(0, dot(vNorm, light_direction));
    vec4 diffuse_color = material_diffuse * light_diffuse * dd1;

    vec4 view_vec = normalize(eye - vPos);
    vec4 bisector = normalize(light_direction + view_vec);
    float dd2 = max(0, dot(vNorm, bisector));
    vec4 specular_color = material_specular * light_specular * pow(dd2, material_shininess);

    vec4 vColor = ambient_color + diffuse_color + specular_color;
    vColor[3] = 1.0;

    gl_Position = MVP * vPos;
    color = vColor;
}
