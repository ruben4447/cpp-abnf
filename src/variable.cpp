#include "../include/variable.hpp"

#include <regex>
#include <string>

#include "../include/utils.hpp"

namespace abnf {
Variable::Variable(const char* name, std::string decl_str) {
    _name = name;
    def_string = decl_str;
}

std::string Variable::get_name() { return _name; }

int Variable::lex(std::string& msg) {
    std::vector<token_t> tokens;

    // Data for next token
    std::string token_type;
    std::vector<std::string> token_data;

    int lim = def_string.length();
    std::string source = def_string;  // Copy of def_string
    int inc = 1;                      // How much to increment pos by?

    for (int pos = 0; pos < lim;) {
        char c = source[0];
        inc = 1;

        // Scan for leading/opening symbols...
        if (token_type == "") {
            if (c == ' ')
                ;
            else if (c == '"') {
                token_type = "string";
                token_data.push_back("");
            } else if (c == '%') {
                // NUMERIC CHARACTER
                token_type = "numeric";
            } else if (c == ';') {
                // Comment
                break;
            } else {
                msg = "error: unknown identification token " + ctos(c);
                return pos;
            }
        } else if (token_type == "string") {
            if (c == '\\' && source.length() > 1) {
                // Escape char
                token_data[0] += c + source[1];
                inc = 2;
            } else if (c == '"') {
                // Terminate string
                _push_token(token_type, token_data, tokens);
            } else {
                // Add to string literal
                token_data[0] += c;
            }
        } else {
            return pos;
        }

        pos += inc;
        source.erase(0, inc);
    }

    // Anything left over?
    if (token_type.length() != 0) {
        msg = "error: unexpected EOL whilst scanning " + token_type;
        return lim;
    }

    _tokens = tokens;
    return -1;
}  // namespace abnf

void Variable::lex_fatal() {
    std::string msg;
    int fpos = lex(msg);
    if (fpos > -1) {
        if (msg.length() == 0) msg = "unknown fatal error";
        msg += " (position " + std::to_string(fpos) + ")";
        def_string += ' ';  // Extra space

        // Print error
        printf("\033[1;31m%s\033[0m\n", msg.c_str());

        // Highlight error'd bit
        std::string norm = "\033[40m\033[37;1m";
        printf("%s%s\033[0m", norm.c_str(), def_string.substr(0, fpos).c_str());
        printf("\033[37;1m\033[41m%c\033[0m", def_string[fpos]);
        if (fpos + 1 < def_string.length())
            printf("%s%s\033[0m", norm.c_str(),
                   def_string.substr(fpos + 1).c_str());

        // Print pointer to error
        printf("\n%s", norm.c_str());
        for (int i = 0; i < fpos; i++) printf(" ");
        printf("\033[1;31m^ \033[0m \n");
        exit(1);
    }
}

// [DEBUG] Print tokens to screen
void Variable::print_tokens() {
    printf("Tokens: \n");
    for (auto token : _tokens) {
        printf("- %s: ", token.first.c_str());
        for (auto t : token.second) printf("`%s` ", t.c_str());
        printf("\n");
    }
}
};  // namespace abnf