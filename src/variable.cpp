#include "../include/variable.hpp"

#include <regex>
#include <string>
#include <vector>

#include "../include/instance.hpp"
#include "../include/token_struct.hpp"
#include "../include/utils.hpp"

namespace abnf {
Variable::Variable(std::string name, bool is_const, std::string decl_str) {
    _name = name;
    _is_const = is_const;
    def_string = decl_str;
}

std::string Variable::get_name() { return _name; }
bool Variable::is_const() { return _is_const; }

// [DEBUG] Print tokens to screen
void Variable::print_tokens() {
    printf("Tokens: \n");
    for (auto token : _tokens) {
        printf("- %s: ", token.type.c_str());
        for (auto item : token.data) printf("`%s` ", item.c_str());
        printf(" (%d - %d)\n", token.spos, token.spos + token.length);
    }
}
};  // namespace abnf