#ifndef HW7_PARSER_H
#define HW7_PARSER_H

#include <vector>
#include <string>

class Parser {

public:
    static int parse_obj (
       const std::string& file_name,
       std::vector<int>& tris,
       std::vector<float>& vertices
    );
};


#endif
