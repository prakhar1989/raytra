#ifndef HW1_PARSER_H
#define HW1_PARSER_H

#include <iostream>
#include <vector>
#include "surface.h"

class Parser {
public:
    static int parse_file(std::string file_name, std::vector<Surface*>& surfaces);
};


#endif //HW1_PARSER_H
