#ifndef HW7_PARSER_H
#define HW7_PARSER_H

#include <vector>

class Parser {

public:
    static int parse_obj (
       std::string file_name,
       std::vector<int>& tris,
       std::vector<float>& vertices
    );
};


#endif
