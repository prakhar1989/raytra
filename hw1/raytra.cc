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

    for (auto s: surfaces) {
        s->print();
        print_color(s->material->diffuse);
    }
}
