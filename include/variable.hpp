#ifndef _VARIABLE_H_
#define _VARIABLE_H_

#include <string>
#include <tuple>
#include <vector>

#include "token_struct.hpp"

namespace abnf {
typedef std::tuple<std::string, std::vector<std::string>, int, int>
    token_t;  // This is used in lex(). { name, data, pos, length }
typedef std::pair<int, int> lex_return_t;  // { spos, length }

class Variable {
   private:
    const char* _name;           // Name of variable
    std::vector<Token> _tokens;  // Result of lex()

   public:
    std::string def_string;  // Definition string

    Variable(const char*, std::string decl_str);
    std::string get_name();  // Get variable name

    void print_tokens();  // Print tokens to screen [DEBUG]

    lex_return_t lex(std::string& msg);  // Lex def_string into tokens. Return
                                         // position of failiure or -1
    void lex_fatal();                    // Same as lex(), but fatal if error
};
};  // namespace abnf

#endif