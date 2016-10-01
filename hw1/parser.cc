#include <fstream>
#include "parser.h"

using namespace std;

int Parser::parse_file(const string file_name)
/* parses a file */
{
    ifstream in(file_name);
    if (!in) {
        cerr << "no file\n";
        return -1;
    }

    for (string line; getline(in, line);) {
        if (line.empty())
            continue;

        switch(line[0]) {
            case '/': continue;
            case 's':
                cout << "found sphere:" << line << endl;
                break;
            case 'c':
                cout << "found camera: " << line << endl;
                break;
            case 'm':
                cout << "found material:" << line << endl;
                break;
            default: continue;
        }
    }
    return 0;
}
