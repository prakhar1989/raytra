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
float theta = 20.0;  // angle with the Z axis
float phi = 90.0;   // angle with the Y axis
float radius = 3.f;  // the distance of camera from origin

const float ZOOMING_STEPS = 0.5f;
const float MAX_DISTANCE_FROM_ORIGIN = -50;
const float MIN_DISTANCE_FROM_ORIGIN = -3;

bool vertex_shader_color_toggle = false;

/** Setting up Light and Material Properties **/
const light_properties light = {
        .position   = {100.0, 100.0, 100.0f, 1.0},
        .ambient    = {0.2, 0.2, 0.2, 1.0},
        .diffuse    = {0.8, 0.8, 0.8, 1.0},
        .specular   = {.5, .5, .5, 1.0}
};

const material_properties material = {
        .ambient    = {1.0, 0.0, 1.0, 1.0},
        .diffuse    = {1.0, 0.8, 0.0, 1.0},
        .specular   = {1.0, 0.8, 0.0, 1.0},
        .shininess  = 100.0
};

const float deg_to_rad = (3.1415926f / 180.0f);

/** values that are sent to shader repeatedly **/
GLint perspective_location, eye_location,
        view_location, show_vs_color_location;

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);

    if (key == GLFW_KEY_Z && action == GLFW_PRESS)
        radius =  fmaxf(radius - ZOOMING_STEPS, -MIN_DISTANCE_FROM_ORIGIN);

    if (key == GLFW_KEY_X && action == GLFW_PRESS)
        radius =  fminf(radius + ZOOMING_STEPS, -MAX_DISTANCE_FROM_ORIGIN);

    if (key == GLFW_KEY_S && action == GLFW_PRESS)
        vertex_shader_color_toggle = !vertex_shader_color_toggle;
}

void init (int n_vertices)
{
    GLuint vertex_buffer, program;

    GLint vpos_location, vnorm_location; // attributes

    // set up vertex buffer object - this will be memory on the GPU where
    // we are going to store our vertex data (that is currently in the "points"
    // array)
    glGenBuffers(1, &vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);

    // specify that its part of a VAO, what its size is, and where the
    // data is located, and finally a "hint" about how we are going to use
    // the data (the driver will put it in a good memory location, hopefully)
    glBufferData(GL_ARRAY_BUFFER, 2 * n_vertices * sizeof(vec4), NULL, GL_DYNAMIC_DRAW);

    program = InitShader("vshader_phong.glsl", "fshader_phong.glsl");
    glUseProgram(program);

    // get access to the various things we will be sending to the shaders:
    perspective_location = glGetUniformLocation(program, "perspective");
    view_location = glGetUniformLocation(program, "view");
    eye_location = glGetUniformLocation(program, "eye");
    show_vs_color_location = glGetUniformLocation(program, "show_vs_color");
    glUniform1i(show_vs_color_location, vertex_shader_color_toggle);

    glUniform4fv(glGetUniformLocation(program, "light_diffuse"), 1, (const GLfloat *) light.diffuse);
    glUniform4fv(glGetUniformLocation(program, "light_specular"), 1, (const GLfloat *) light.specular);
    glUniform4fv(glGetUniformLocation(program, "light_ambient"), 1, (const GLfloat *) light.ambient);
    glUniform4fv(glGetUniformLocation(program, "light_position"), 1, (const GLfloat *) light.position);
    glUniform4fv(glGetUniformLocation(program, "material_diffuse"), 1, (const GLfloat *) material.diffuse);
    glUniform4fv(glGetUniformLocation(program, "material_specular"), 1, (const GLfloat *) material.specular);
    glUniform4fv(glGetUniformLocation(program, "material_ambient"), 1, (const GLfloat *) material.ambient);
    glUniform1f(glGetUniformLocation(program, "material_shininess"), material.shininess);

    vpos_location = glGetAttribLocation(program, "vPos");
    vnorm_location = glGetAttribLocation(program, "vNorm");

    glEnableVertexAttribArray(vpos_location);
    glVertexAttribPointer(vpos_location, 4, GL_FLOAT, GL_FALSE, 0, (void*) (0));

    glEnableVertexAttribArray(vnorm_location);
    glVertexAttribPointer(vnorm_location, 4, GL_FLOAT, GL_FALSE, 0, (void*) (n_vertices * sizeof(vec4)));
}

static void mouse_move_rotate (GLFWwindow* window, double x, double y)
{
    static double last_x = x; // last pos of mouse in x
    static double last_y = y; // last pos of mouse in y

    double delta_x = x - last_x;
    if (delta_x != 0) {
        theta += delta_x;
        if (theta > 360.0 ) theta -= 360.0;
        if (theta < 0.0 ) theta += 360.0;
        last_x = x;
    }

    double delta_y = y - last_y;
    if (delta_y != 0) {
        phi += delta_y;
        if (phi > 175.0) phi = 175.0;
        if (phi < 5.0) phi = 5.0;
        last_y = y;
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
    std::vector<my_vec> normals;
    const auto n_vertices = Parser::parse_obj(obj_file, verts, normals);

    point4 vertices[n_vertices];
    point4 points[n_vertices];
    vec4 norms[n_vertices];

    for (auto i = 0; i < n_vertices; i++) {
        points[i][0] = verts[3*i];
        points[i][1] = verts[3*i+1];
        points[i][2] = verts[3*i+2];
        points[i][3] = 1;

        norms[i][0] = normals[i].x;
        norms[i][1] = normals[i].y;
        norms[i][2] = normals[i].z;
        norms[i][3] = 0;
    }

    if (!glfwInit())
    {
        fprintf(stderr, "Failed to initialize GLFW\n");
        exit(EXIT_FAILURE);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_SAMPLES, 4);

    GLFWwindow* window = glfwCreateWindow(640, 480, "GLRender v0.1", NULL, NULL);
    if (!window)
    {
        fprintf(stderr, "Failed to initialize glfw window\n");
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwSetErrorCallback(error_callback);
    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, mouse_move_rotate);

    glfwMakeContextCurrent(window);
    gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);
    glfwSwapInterval(1);

    /** Open GL Init **/
    init(n_vertices);

    /** Enable Z Buffering for depth */
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    /** Move data over to the shader **/
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(points), points);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(points), sizeof(norms), norms);

    while (!glfwWindowShouldClose(window))
    {
        float ratio;
        int width, height;
        mat4x4 projection;
        mat4x4 view;

        // update with window viewport size
        glfwGetFramebufferSize(window, &width, &height);
        ratio = width / (float) height;

        glViewport(0, 0, width, height);
        glClearColor(1.0, 1.0, 1.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // camera location
        vec3 eye = {
            radius * sinf(phi * deg_to_rad) * sinf(theta * deg_to_rad),
            radius * cosf(phi * deg_to_rad),
            radius * sinf(phi * deg_to_rad) * cosf(theta * deg_to_rad)
        };

        vec3 up = {0, 1.f, 0};
        vec3 center = {0, 0, 0};
        mat4x4_look_at(view, eye, center, up);
        mat4x4_perspective(projection, 30 * deg_to_rad, ratio, 0.1f, 100.f);

        vec4 viewer = {eye[0], eye[1], eye[2], 1.f};
        glUniform4fv(eye_location, 1, (const GLfloat *) viewer);
        glUniform1i(show_vs_color_location, vertex_shader_color_toggle);

        glUniformMatrix4fv(perspective_location, 1, GL_FALSE, (const GLfloat*) projection);
        glUniformMatrix4fv(view_location, 1, GL_FALSE, (const GLfloat*) view);
        glDrawArrays(GL_TRIANGLES, 0, n_vertices);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);

    glfwTerminate();
    exit(EXIT_SUCCESS);
}
