#include "../include/variable.hpp"

#include <regex>
#include <string>
#include <vector>

#include "../include/token_struct.hpp"
#include "../include/utils.hpp"

namespace abnf {
Variable::Variable(const char* name, std::string decl_str) {
    _name = name;
    def_string = decl_str;
}

std::string Variable::get_name() { return _name; }

/**
 *  String: { "string", { flag, str } }
 *  Numeric { "numeric", { base, num, ... } }
 *  Numeric Range: { "numeric_range", { base, lower, upper } }
 */
lex_return_t Variable::lex(std::string& msg) {
    std::vector<Token> tokens;

    // Data for next token
    struct Token token = {};

    int lim = def_string.length();
    std::string source = def_string;  // Copy of def_string
    int inc;                          // How much to increment pos by?

    for (int pos = 0; pos < lim;) {
        char c = source[0];
        inc = 1;

        // Scan for leading/opening symbols...
        if (token.type == "") {
            if (c == ' ')
                ;  // Skip whitespace
            else if (c == '"') {
                // STRING
                token.type = "string";
                if (token.data.size() == 0)
                    // Default flag: case insentitive
                    token.data.push_back("i");
                token.data.push_back("");
            } else if (c == '%') {
                // NUMERIC CHARACTER
                token.type = "percent";
            } else if (c == ';') {
                // Comment
                break;
            } else if (is_alpha(c)) {
                // VARIABLE?
                token.type = "identifier";
                token.data.push_back(ctos(c));
            } else if (c == '<') {
                // VARIABLE enclosed in <>
                token.type = "identifier";
                token.data.push_back("");
            } else {
                msg = "error: unknown identification token " + ctos(c);
                return {pos, 1};
            }
            if (token.spos == 0) token.spos = pos;  // Set starting position
        } else if (token.type == "string") {
            if (c == '\\' && source.length() > 1) {
                // Escape char
                token.data[1] += c + source[1];
                inc = 2;
            } else if (c == '"') {
                tokens.push_back(token);
                token = {};
            } else {
                // Add to string literal
                token.data[1] += c;
            }
        } else if (token.type == "percent") {
            if (source.length() == 1) {
                msg = "error: unexpected end of input";
                return {pos, 1};
            } else if (c == 'b' || c == 'd' || c == 'x') {
                char d = source[1];
                if (digit_match_base(c, d)) {
                    // Numeric Base
                    token.type = "numeric";
                    token.data.push_back(ctos(c));
                    token.data.push_back(ctos(d));
                    inc = 2;
                } else {
                    msg = "error: invalid literal for base " + ctos(c) + ": " +
                          ctos(d);
                    return {pos + 1, 1};
                }
            } else if (c == 's' || c == 'i') {
                // String flag
                if (source[1] == '"') {
                    token.type = "";
                    token.data.push_back(ctos(c));
                    // -- Carried forward to "string"
                } else {
                    msg =
                        "error: expected string to follow string flag: "
                        "expected \", got " +
                        ctos(source[1]);
                    return {pos - 1, 3};
                }
            } else {
                msg = "error: invalid token succeeding %: " + ctos(c);
                return {pos, 1};
            }
        } else if (token.type == "numeric") {
            // Digit?
            char base = token.data[0][0];
            if (digit_match_base(base, c)) {
                *get_last(token.data) += c;
            } else if (c == '.') {
                // Concatenate (expect digit)
                if (get_last(token.data)->length() == 0) {
                    msg = "error: invalid base-" + ctos(base) +
                          " digit: " + ctos(c);
                    return {pos, 1};
                } else {
                    // Start new numeric literal
                    token.data.push_back("");
                }
            } else if (c == '-') {
                // Range
                token.data.push_back("");
                token.type = "numeric_range";
            } else {
                tokens.push_back(token);
                token = {};
            }
        } else if (token.type == "numeric_range") {
            char base = token.data[0][0];
            if (digit_match_base(base, c)) {
                *get_last(token.data) += c;
            } else {
                // There must be something here
                if (get_last(token.data)->length() == 0) {
                    msg = "error: invalid base-" + ctos(base) +
                          " digit: " + ctos(c);
                    return {pos, 1};
                } else {
                    tokens.push_back(token);
                    token = {};
                }
            }
        } else if (token.type == "identifier") {
            if (is_alpha(c) || c == '_' || c == '-') {
                token.data[0] += c;
            } else {
                if (c == '>') inc++;  // Hop over this
                tokens.push_back(token);
                token = {};
            }
        } else {
            msg = "error: unknown token type " + token.type;
            return {pos, 1};
        }

        // Increment
        if (inc > 0) {
            pos += inc;
            source.erase(0, inc);
            token.length += inc;
        }
    }

    // Anything left over?
    if (token.type == "string" ||
        (token.type == "numeric" && *get_last(token.data) == "") ||
        (token.type == "numeric_range" &&
         (token.data.size() != 3 || *get_last(token.data) == ""))) {
        // If string, or unfinished concatenation, or unfinished range...
        msg = "error: unexpected EOL whilst scanning " + token.type;
        return {lim, 2};
    } else if (token.type != "") {
        // Push trailing token
        tokens.push_back(token);
        token = {};
    }

    // ============================= Post-Lexer Checking/Tidying
    // - Convert all numbers to Base 10 and remove base char
    // - Check numerical_ranges are valid
    int token_count = tokens.size();
    for (Token& token : tokens) {
        // Convert to decimal
        if (token.type == "numeric" || token.type == "numeric_range") {
            // Don't bother with decimal->decimal
            if (token.data[0][0] != 'd') {
                int base = charbase_to_int(token.data[0][0]);
                int nums = token.data.size();
                for (int j = 1; j < nums; j++) {
                    token.data[j] =
                        std::to_string(std::stoi(token.data[j], nullptr, base));
                }
            }
            // Remove base
            token.data.erase(token.data.begin());
        }

        // Check range
        if (token.type == "numeric_range") {
            int lower = std::stoi(token.data[0]),
                upper = std::stoi(token.data[1]);
            if (upper <= lower) {
                msg = "error: negative range " + token.data[0] + "-" +
                      token.data[1];
                return {token.spos, token.length};
            }
        }
    }

    // - Concatenate sequential strings with same flags
    for (int i = 0; i < tokens.size() - 1;) {
        // Both strings and flags are equal
        if (tokens[i].type == "string" && tokens[i + 1].type == "string" &&
            tokens[i].data[0][0] == tokens[i + 1].data[0][0]) {
            // Concatenate strings
            tokens[i].data[1] += tokens[i + 1].data[1];
            tokens.erase(tokens.begin() + i + 1);
        } else {
            // Increment i
            i++;
        }
    }

    _tokens = tokens;
    return {-1, 0};
}

void Variable::lex_fatal() {
    std::string msg;
    lex_return_t val = lex(msg);  // { pos, len } of error
    if (val.first > -1) {
        throw_error(def_string, msg, val.first, val.first + val.second);
    }
}

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