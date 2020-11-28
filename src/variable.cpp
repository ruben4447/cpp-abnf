#include <string>
#include "../include/variable.hpp"

namespace abnf {
    Variable::Variable(std::string name, std::string decl_str) {
        _name = name;
        _decl_str = decl_str;
    }

    std::string Variable::get_name() { return _name; }
    std::string Variable::get_decl_str() { return _decl_str; }

    void Variable::concat_decl(std::string decl_str) { _decl_str += "/ " + decl_str; }
};