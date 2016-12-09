#version 120

// light properties
uniform vec4 light_diffuse;
uniform vec4 light_specular;
uniform vec4 light_ambient;
uniform vec4 light_position;

varying vec4 f_vNorm;
varying vec4 f_light_direction;
varying vec4 f_view_vec;
// world space position
varying vec4 pos;

vec4 compute_shading(vec4 material_ambient, vec4 material_specular,
                     vec4 material_diffuse, float material_shininess)
{
    vec4 ambient_color = material_ambient * light_ambient;
    float dd1 = max(0, dot(f_vNorm, f_light_direction));
    vec4 diffuse_color = material_diffuse * light_diffuse * dd1;

    vec4 bisector = normalize(f_light_direction + f_view_vec);
    float dd2 = max(0, dot(f_vNorm, bisector));
    vec4 specular_color = material_specular * light_specular * pow(dd2, material_shininess);

    vec4 f_color = ambient_color + diffuse_color + specular_color;
    f_color[3] = 1.0;

    return f_color;
}

vec4 checker(float u, float v, float z)
{
    vec4 black_material_ambient = vec4(0.0, 0.0, 0.0, 1.0);
    vec4 black_material_diffuse = vec4(0.03, 0.03, 0.03, 1.0);
    vec4 black_material_specular = vec4(0.1, 0.1, 0.1, 1.0);
    float black_material_shininess = 100.0;

    vec4 white_material_ambient = vec4(0.05, 0.05, 0.05, 1.0);
    vec4 white_material_diffuse = vec4(0.95, 0.95, 0.95, 1.0);
    vec4 white_material_specular = vec4(0.5, 0.5, 0.5, 1.0);
    float white_material_shininess = 100.0;

    float checksize = 20;
    float res = mod(floor(checksize * u) + floor(checksize * v), 2.0);

    if (res == 0)
        return compute_shading(black_material_ambient, black_material_specular,
                                black_material_diffuse, black_material_shininess);

    return compute_shading(white_material_ambient, white_material_specular,
                           white_material_diffuse, white_material_shininess);
}

void main()
{
    gl_FragColor = checker(pos.x, pos.y, pos.z);
}
