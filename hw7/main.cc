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

const int NumVertices = 3;

point4 vertices[3] = {
        { -0.25,0.0,0.0, 1.0},
        { 0.25,0.0,0.0, 1.0},
        { 0.0, 0.5,0.0, 1.0}
};

// viewer's position, for lighting calculations
point4 viewer = {0.0, 0.0, -1.0, 1.0};

// we will copy our transformed points to here:
point4 points[NumVertices];

// and we will store the colors, per face per vertex, here. since there is
// only 1 triangle, with 3 vertices, there will just be 3 here:
color4 colors[NumVertices];

// a transformation matrix, for the rotation, which we will apply to every
// vertex:
mat4x4 ctm;

float theta = 0.0;  // mouse rotation around the Y (up) axis
float posx = 0.0;   // translation along X
float posy = 0.0;   // translation along Y


// transform the triangle's vertex data and put it into the points array.
// also, compute the lighting at each vertex, and put that into the colors
// array.
void tri(light_properties& light, material_properties& material)
{
    // compute the lighting at each vertex, then set it as the color there:

    mat4x4_mul_vec4 (points[0], ctm, vertices[0]);
    mat4x4_mul_vec4 (points[1], ctm, vertices[1]);
    mat4x4_mul_vec4 (points[2], ctm, vertices[2]);

    vec4 e1;
    vec4_sub(e1, points[1], points[0]);

    vec4 e2;
    vec4_sub(e2, points[2], points[0]);
    vec4 n1;
    vec4_mul_cross(n1, e1, e2);
    vec4_norm(n1, n1);

    vec4 n = {n1[0], n1[1], n1[2], 1.0};
    vec4 light_pos_viewer;
    vec4_add (light_pos_viewer, light.position, viewer);
    vec4 half;
    vec4_norm(half, light_pos_viewer);

    color4 ambient_color, diffuse_color, specular_color;

    vecproduct(ambient_color, material.ambient, light.ambient);

    float dd = vec4_mul_inner(light.position, n);

    if(dd>0.0) {
        color4 diffuse_product;
        vecproduct(diffuse_product, light.diffuse, material.diffuse);
        vec4_scale (diffuse_product, diffuse_product, dd);
        vecset(diffuse_color, diffuse_product);
    }
    else
        vecclear(diffuse_color);

    dd = vec4_mul_inner(half, n);

    if(dd > 0.0) {
        color4 spec_prod;
        vecproduct(spec_prod, light.specular, material.specular);
        vec4_scale (spec_prod,spec_prod, exp(material.shininess*log(dd)));
        vecset(specular_color, spec_prod);
    }
    else
        vecclear(specular_color);

    vec4_add (colors[0], ambient_color, diffuse_color);
    vec4_add (colors[0], colors[0], specular_color);

    vec4_add (colors[1], ambient_color, diffuse_color);
    vec4_add (colors[1], colors[1], specular_color);

    vec4_add (colors[2], ambient_color, diffuse_color);
    vec4_add (colors[2], colors[2], specular_color);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}

void init(GLint& mvp_location)
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
    glBufferData(GL_ARRAY_BUFFER, sizeof(points) + sizeof(colors),
                 vertices, GL_DYNAMIC_DRAW);

    // load in these two shaders...  (note: InitShader is defined in the
    // accompanying initshader.c code).
    // the shaders themselves must be text glsl files in the same directory
    // as we are running this program:
    program = InitShader("vshader_passthrough_lit.glsl", "fshader_passthrough_lit.glsl");

    // ...and set them to be active
    glUseProgram(program);

    // get acces to the various things we will be sending to the shaders:
    mvp_location = glGetUniformLocation(program, "MVP");
    vpos_location = glGetAttribLocation(program, "vPos");
    vcol_location = glGetAttribLocation(program, "vCol");

    glEnableVertexAttribArray(vpos_location);

    // the vPosition attribute is a series of 4-vecs of floats, starting at the
    // beginning of the buffer
    glVertexAttribPointer(vpos_location, 4, GL_FLOAT, GL_FALSE,
                          0, (void*) (0));


    glEnableVertexAttribArray(vcol_location);
    glVertexAttribPointer(vcol_location, 4, GL_FLOAT, GL_FALSE,
                          0, (void*) (sizeof(points)));
}

// use this motionfunc to demonstrate rotation - it adjusts "theta" based
// on how the mouse has moved.
static void mouse_move_rotate (GLFWwindow* window, double x, double y)
{

    static int lastx = 0;// keep track of where the mouse was last:

    int amntX = x - lastx;
    if (amntX != 0) {
        theta +=  amntX;
        if (theta > 360.0 ) theta -= 360.0;
        if (theta < 0.0 ) theta += 360.0;

        lastx = x;
    }

    //  std::cout << theta << std::endl;
}

// use this motionfunc to demonstrate translation - it adjusts posx and
// posy based on the mouse movement. posx and posy are then used in the
// display loop to generate the transformation that is applied
// to all the vertices before they are displayed:
static void mouse_move_translate (GLFWwindow* window, double x, double y)
{

    static int lastx = 0;
    static int lasty = 0;

    // if we want relative motion, keep track of where the mouse was last:
//
//    if (x - lastx < 0) --posx;
//    else if (x - lastx > 0) ++posx;
//    lastx = x;
//
//    if (y - lasty < 0) --posy;
//    else if (y - lasty > 0) ++posy;
//    lasty = y;

    posx = x;
    posy = -y;
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
    std::vector<int> tris;
    std::vector<float> verts;
    Parser::parse_obj(obj_file, tris, verts);

    // build an array of myvertices
    auto n_vertices = verts.size()/3;
    point4 myvertices[n_vertices];

    for (auto i = 0; i < n_vertices; i++) {
        myvertices[i][0] = verts[3*i];
        myvertices[i][1] = verts[3*i + 1];
        myvertices[i][2] = verts[3*i + 2];
        myvertices[i][3] = 1.0;
    }

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

    GLint mvp_location;
    init(mvp_location);

    light_properties light = {
            .position   = {0.0, 0.0, -1.0, 1.0},
            .ambient    = {0.2, 0.2, 0.2, 1.0},
            .diffuse    = {1.0, 1.0, 1.0, 1.0},
            .specular   = {1.0, 1.0, 1.0, 1.0}
    };

    material_properties material = {
            .ambient    = {1.0, 0.0, 1.0, 1.0},
            .diffuse    = { 1.0, 0.8, 0.0, 1.0 },
            .specular   = { 1.0, 0.8, 0.0, 1.0 },
            .shininess  = 100.0
    };


    while (!glfwWindowShouldClose(window))
    {
        float ratio;
        int width, height;
        mat4x4 p;

        // in case the  window viewport size changed, we will need to adjust the
        // projection:
        glfwGetFramebufferSize(window, &width, &height);
        ratio = width / (float) height;

        glViewport(0, 0, width, height);

        // clear the window (with white) and clear the z-buffer
        glClearColor(1.0, 1.0, 1.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // make up a transform that rotates around screen "Z" with time:
        mat4x4_identity(ctm);
        mat4x4_rotate_Y(ctm, ctm, theta / 180.0);

        // tri() will multiply the points by ctm, and figure out the lighting
        tri(light, material);

        // tell the VBO to re-get the data from the points and colors arrays:
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(points), points);
        glBufferSubData(GL_ARRAY_BUFFER, sizeof(points), sizeof(colors), colors);

        // orthographically project to screen:
        mat4x4_ortho(p, -ratio, ratio, -1.f, 1.f, 1.f, -1.f);

        glUniformMatrix4fv(mvp_location, 1, GL_FALSE, (const GLfloat*) p);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);

    glfwTerminate();
    exit(EXIT_SUCCESS);
}
