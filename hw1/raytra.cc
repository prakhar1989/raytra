#include "parser.h"

using namespace std;

void print_color(const Raytra::color& c)
{
    printf("r: %f, g: %f, b: %f\n", c.red, c.green, c.blue);
}

int main(int argc, char** argv)
{
    if (argc < 2) {
        cerr << "USAGE: raytra <scene_file> <output_file>" << endl;
        return -1;
    }

    auto version = "0.1";

    string scene_file {argv[1]};
    vector<Surface*> surfaces;
    Camera* camera;

    Parser::parse_file(scene_file, surfaces, camera);

    // TODO: for debugging only
    for (auto s: surfaces) {
        s->print();
        print_color(s->material->diffuse);
    }

    for (int i = 0; i < camera->nx; i++) {
        for (int j = 0; j < camera->ny; j++) {

            /* Step 1. Ray Generation */
            float u = camera->left +
                    (camera->right - camera->left) * (i + 0.5f)/camera->nx;
            float v = camera->bottom +
                      (camera->top - camera->bottom) * (j + 0.5f)/camera->ny;

            Raytra::vector dir = (-camera->focal_length * camera->w) +
                                (u * camera->u) +  (v * camera->v);

            Raytra::Point origin = camera->eye;

            Ray* r = new Ray(origin, dir);
        }
    }
}
