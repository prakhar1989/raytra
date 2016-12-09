#version 120

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

uniform bool show_vs_color;

varying vec4 f_vNorm;
varying vec4 f_light_direction;
varying vec4 f_view_vec;
varying vec4 color; // passed by vertex shader

void main()
{
    vec4 ambient_color = material_ambient * light_ambient;
    float dd1 = max(0, dot(f_vNorm, f_light_direction));
    vec4 diffuse_color = material_diffuse * light_diffuse * dd1;

    vec4 bisector = normalize(f_light_direction + f_view_vec);
    float dd2 = max(0, dot(f_vNorm, bisector));
    vec4 specular_color = material_specular * light_specular * pow(dd2, material_shininess);

    vec4 f_color = ambient_color + diffuse_color + specular_color;
    f_color[3] = 1.0;

    if (show_vs_color) {
        gl_FragColor = f_color;
    } else {
        gl_FragColor = color;
    }
}

