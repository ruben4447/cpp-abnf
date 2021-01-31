#ifndef _VARIABLE_H_
#define _VARIABLE_H_

#include <string>
#include <tuple>
#include <vector>

#include "token_struct.hpp"

namespace abnf {
// typedef std::tuple<std::string, std::vector<std::string>, int, int>
//     token_t;  // This is used in lex(). { name, data, pos, length }
typedef std::pair<int, int> int_pair_t;  // { spos, length }

class Variable {
   private:
    std::string _name;  // Name of variable
    bool _is_const;     // Can be redefined?

   public:
    bool lexed = false;
    std::string def_string;      // Definition string
    std::vector<Token> _tokens;  // Result of lex()

    Variable(std::string, bool, std::string);
    std::string get_name();  // Get variable name
    bool is_const();

    void print_tokens();  // Print tokens to screen [DEBUG]

    int_pair_t lex(std::string& msg);  // Lex def_string into tokens. Return
                                       // position of failiure or -1
    void lex_fatal();                  // Same as lex(), but fatal if error

    bool test(std::string&, std::string&, int&);
};
};  // namespace abnf

#endif