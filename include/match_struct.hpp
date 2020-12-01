#ifndef _MATCH_STRUCT_H_
#define _MATCH_STRUCT_H_

#include <string>
#include <vector>

namespace abnf {
    struct match_return {
        bool ok; // Matched?
        std::string msg; // Message
        std::vector<std::string> matches; // Return of what matched
    };
};

#endif