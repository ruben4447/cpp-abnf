#ifndef _TOKEN_STRUCT_H_
#define _TOKEN_STRUCT_H_

#include <string>
#include <vector>

namespace abnf {
struct Token {
    std::string type;               // Token type
    std::vector<std::string> data;  // Data
    int spos;                       // Start position (for errors)
    int length;                     // Length of raw content
};
};  // namespace abnf

#endif