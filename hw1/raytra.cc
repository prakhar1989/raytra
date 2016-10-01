#include "parser.h"
#include "surface.h"
#include <iostream>
#include <vector>

using namespace std;

int main(int argc, char** argv)
{
    if (argc < 2) {
        cerr << "USAGE: raytra <scene_file> <output_file>" << endl;
        return -1;
    }

    auto version = "0.1";
    printf("Welcome to Raytra v%s\n", version);

    string scene_file {argv[1]};
    vector<Surface*> surfaces;

    Parser::parse_file(scene_file, surfaces);

    for (auto s: surfaces)
        s->print();
}
