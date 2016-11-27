#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "linmath.h"
#include <stdlib.h>
#include <vector>
#include <stdio.h>
#include "utils.h"
#include <iostream>
#include "parser.h"

using namespace std;

// source provided for function to load and compile shaders
GLuint InitShader(const char* vertexShaderFile, const char* fragmentShaderFile);

/** Global location config **/
float theta = 0.0;   // mouse rotation around the Y (up) axis
double posx = 0.0;   // translation along X
double posy = 0.0;   // translation along Y

const float deg_to_rad = (3.1415926f / 180.0f);

// transform the triangle's vertex data and put it into the points array.
// also, compute the lighting at each vertex, and put that into the colors
// array.
void transform (
        const point4& viewer,
        const light_properties& light,
        const material_properties& material,
        point4 vertices[], point4 points[], color4 colors[],
        mat4x4& rotation_mat,
        unsigned long n_vertices
    )
{
    for (unsigned int i = 0; i < n_vertices/3; i++) {
        auto a_index = 3 * i;
        auto b_index = 3 * i + 1;
        auto c_index = 3 * i + 2;

        mat4x4_mul_vec4(points[a_index], rotation_mat, vertices[a_index]);
        mat4x4_mul_vec4(points[b_index], rotation_mat, vertices[b_index]);
        mat4x4_mul_vec4(points[c_index], rotation_mat, vertices[c_index]);

        // compute the triangle norm
        vec4 e1, e2, n1, light_pos_viewer, half;
        color4 ambient_color, diffuse_color, specular_color;

        vec4_sub(e1, vertices[b_index], vertices[a_index]);
        vec4_sub(e2, vertices[c_index], vertices[a_index]);
        vec4_mul_cross(n1, e1, e2);
        vec4_norm(n1, n1);

        vec4 n = {n1[0], n1[1], n1[2], 1.0};
        vec4_add(light_pos_viewer, light.position, viewer);
        vec4_norm(half, light_pos_viewer);

        vecproduct(ambient_color, material.ambient, light.ambient);

        // calculate the diffuse shading
        float dd = vec4_mul_inner(light.position, n);
        if (dd > 0.0) {
            color4 diffuse_product;
            vecproduct(diffuse_product, light.diffuse, material.diffuse);
            vec4_scale(diffuse_product, diffuse_product, dd);
            vecset(diffuse_color, diffuse_product);
        } else {
            vecclear(diffuse_color);
        }

        // calculate the specular shading
        dd = vec4_mul_inner(half, n);
        if (dd > 0.0) {
            color4 spec_prod;
            vecproduct(spec_prod, light.specular, material.specular);
            vec4_scale(spec_prod, spec_prod, exp(material.shininess * log(dd)));
            vecset(specular_color, spec_prod);
        } else {
            vecclear(specular_color);
        }

        // set the computed colors
        vec4_add(colors[a_index], ambient_color, diffuse_color);
        vec4_add(colors[a_index], colors[a_index], specular_color);

        vec4_add(colors[b_index], ambient_color, diffuse_color);
        vec4_add(colors[b_index], colors[b_index], specular_color);

        vec4_add(colors[c_index], ambient_color, diffuse_color);
        vec4_add(colors[c_index], colors[c_index], specular_color);
    }
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}

void init (GLint& mvp_location, point4 vertices[], int n_colors, int n_points)
{
    // "names" for the various buffers, shaders, programs etc:
    GLuint vertex_buffer, program;
    GLint vpos_location, vcol_location;

    // set up vertex buffer object - this will be memory on the GPU where
    // we are going to store our vertex data (that is currently in the "points"
    // array)
    glGenBuffers(1, &vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);

    // specify that its part of a VAO, what its size is, and where the
    // data is located, and finally a "hint" about how we are going to use
    // the data (the driver will put it in a good memory location, hopefully)
    glBufferData(GL_ARRAY_BUFFER, (n_colors + n_points) * sizeof(vec4), NULL, GL_DYNAMIC_DRAW);

    program = InitShader("shaders/vshader_passthrough_lit.glsl",
                         "shaders/fshader_passthrough_lit.glsl");

    glUseProgram(program);

    // get access to the various things we will be sending to the shaders:
    mvp_location  = glGetUniformLocation(program, "MVP");
    vpos_location = glGetAttribLocation(program, "vPos");
    vcol_location = glGetAttribLocation(program, "vCol");

    glEnableVertexAttribArray(vpos_location);

    // the vPosition attribute is a series of 4-vecs of floats, starting at the
    // beginning of the buffer
    glVertexAttribPointer(vpos_location, 4, GL_FLOAT, GL_FALSE,
                          0, (void*) (0));

    glEnableVertexAttribArray(vcol_location);
    glVertexAttribPointer(vcol_location, 4, GL_FLOAT, GL_FALSE,
                          0, (void*) (n_points * sizeof(vec4)));
}

// use this motionfunc to demonstrate rotation - it adjusts "theta" based
// on how the mouse has moved.
static void mouse_move_rotate (GLFWwindow* window, double x, double y)
{
    static double last_x = 0;// keep track of where the mouse was last:
    double delta = x - last_x;
    if (delta != 0) {
        theta += delta;
        if (theta > 360.0 ) theta -= 360.0;
        if (theta < 0.0 ) theta += 360.0;
        last_x = x;
    }
}


int main(int argc, char* argv[])
{
    if (argc < 2) {
        std::cerr << "USAGE: glrender <obj_file>" << std::endl;
        exit(EXIT_FAILURE);
    }

    std::string obj_file {argv[1]};

    if (!does_file_exist(obj_file)) {
        std::cerr << "Obj file not found" << std::endl;
        exit(EXIT_FAILURE);
    }

    // read in the object file
    std::vector<float> verts;
    const auto n_vertices = Parser::parse_obj(obj_file, verts);

    // collection of vertices as OpenGL expects them
    point4 vertices[n_vertices];

    for (auto i = 0; i < n_vertices; i++) {
        vertices[i][0] = verts[3*i];
        vertices[i][1] = verts[3*i+1];
        vertices[i][2] = verts[3*i+2];
        vertices[i][3] = 1;
    }

    // we will copy our transformed points to here:
    point4 points[n_vertices];

    // and we will store the colors, per face per vertex, here. since there is
    // only 1 triangle, with 3 vertices, there will just be 3 here:
    color4 colors[n_vertices];

    // if there are errors, call this routine:
    glfwSetErrorCallback(error_callback);

    // start up GLFW:
    if (!glfwInit())
        exit(EXIT_FAILURE);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

    // for more modern version of OpenGL:
    //  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    //  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    //  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    //  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);

    GLFWwindow* window;
    window = glfwCreateWindow(640, 480, "GLRender v0.1", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwSetKeyCallback(window, key_callback);

    // call only once: demo for rotation:
    glfwSetCursorPosCallback(window, mouse_move_rotate);

    glfwMakeContextCurrent(window);
    gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);
    glfwSwapInterval(1);

    /** Open GL Init **/
    GLint mvp_location;
    init(mvp_location, &vertices[0], n_vertices, n_vertices);

    /** Enable Z Buffering for depth */
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    /** Setting up Light and Material Properties **/
    light_properties light = {
        .position   = {0.0, 0.0, -1.0f, 1.0},
        .ambient    = {0.2, 0.2, 0.2, 1.0},
        .diffuse    = {1.0, 1.0, 1.0, 1.0},
        .specular   = {1.0, 1.0, 1.0, 1.0}
    };

    material_properties material = {
        .ambient    = {1.0, 0.0, 1.0, 1.0},
        .diffuse    = {1.0, 0.8, 0.0, 1.0},
        .specular   = {1.0, 0.8, 0.0, 1.0},
        .shininess  = 100.0
    };

    /** Set the viewer's location **/
    point4 viewer = {0.0, 0.0, -1.0, 1.0};

    // a transformation matrix, for the rotation,
    // which we will apply to every vertex
    mat4x4 rotation_mat;

    while (!glfwWindowShouldClose(window))
    {
        float ratio;
        int width, height;
        mat4x4 p;

        // update with window viewport size
        glfwGetFramebufferSize(window, &width, &height);
        ratio = width / (float) height;

        glViewport(0, 0, width, height);

        // clear the window (with white) and clear the z-buffer
        glClearColor(1.0, 1.0, 1.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // make up a transform that rotates around screen "Z" with time:
        mat4x4_identity(rotation_mat);
        mat4x4_rotate_Y(rotation_mat, rotation_mat, theta * deg_to_rad);

        // transform() will multiply the points by rotation_mat, and figure out the lighting
        transform(viewer, light, material,
            &vertices[0], &points[0],
            &colors[0], rotation_mat, n_vertices);

        // tell the VBO to re-get the data from the points and colors arrays:
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(points), points);
        glBufferSubData(GL_ARRAY_BUFFER, sizeof(points), sizeof(colors), colors);

        // orthographically project to screen:
        mat4x4_ortho(p, -ratio, ratio, -1.f, 1.f, 1.f, -1.f);

        glUniformMatrix4fv(mvp_location, 1, GL_FALSE, (const GLfloat*) p);
        glDrawArrays(GL_TRIANGLES, 0, n_vertices);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);

    glfwTerminate();
    exit(EXIT_SUCCESS);
}
