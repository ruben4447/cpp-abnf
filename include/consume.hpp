#ifndef _CONSUME_H_
#define _CONSUME_H_

#include <string>

namespace abnf {
void consume_whitespace(std::string& str);
bool consume_string(std::string& input, std::string& value);
int consume_int(std::string& input);
};  // namespace abnf

#endif