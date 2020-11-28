#include "../include/variable.hpp"

#include <string>

namespace abnf {
Variable::Variable(std::string name, std::string decl_str) {
    _name = name;
    def_string = decl_str;
}

std::string Variable::get_name() { return _name; }
};  // namespace abnf