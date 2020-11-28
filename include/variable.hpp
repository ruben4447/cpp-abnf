#ifndef _VARIABLE_H_
#define _VARIABLE_H_

#include <string>

namespace abnf {
class Variable {
   private:
    std::string _name;  // Name of variable

   public:
    std::string def_string;  // Definition string

    std::string get_name();  // Get variable name

    Variable(std::string name, std::string decl_str);
};
};  // namespace abnf

#endif