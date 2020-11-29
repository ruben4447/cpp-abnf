#ifndef _VARIABLE_H_
#define _VARIABLE_H_

#include <string>
#include <vector>

namespace abnf {
typedef std::pair<std::string, std::vector<std::string>>
    token_t;  // This is used in lex(). { name, data }

class Variable {
   private:
    const char* _name;             // Name of variable
    std::vector<token_t> _tokens;  // Result of lex()

   public:
    std::string def_string;  // Definition string

    Variable(const char*, std::string decl_str);
    std::string get_name();  // Get variable name

    void print_tokens();  // Print tokens to screen [DEBUG]

    int lex(std::string& msg);  // Lex def_string into tokens. Return position
                                // of failiure or -1
    void lex_fatal();           // Same as lex(), but fatal if error
};
};  // namespace abnf

#endif