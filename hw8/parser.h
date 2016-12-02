#ifndef HW7_PARSER_H
#define HW7_PARSER_H

#include <vector>
#include <string>

class Parser {

public:
    static unsigned long parse_obj (
       const std::string& file_name,
       std::vector<float>& vertices
    );
};


#endif
