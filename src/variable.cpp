#include "../include/variable.hpp"

#include <regex>
#include <string>
#include <vector>

#include "../include/token_struct.hpp"
#include "../include/utils.hpp"
#include "../include/match_struct.hpp"

namespace abnf {
Variable::Variable(const char* name, bool is_const, std::string decl_str) {
    _name = name;
    _is_const = is_const;
    def_string = decl_str;
}

std::string Variable::get_name() { return _name; }
bool Variable::is_const() { return _is_const; }

/**
 *  String: { "string", { flag, str } }
 *  Numeric { "numeric", { base, num, ... } }
 *  Numeric Range: { "numeric_range", { base, lower, upper } }
 *  Repetition: { "repetition", { min, max } }
 */
int_pair_t Variable::lex(std::string& msg) {
    std::vector<Token> tokens;

    // Data for next token
    struct Token token = {};

    int lim = def_string.length();
    std::string source = def_string;  // Copy of def_string
    int inc;                          // How much to increment pos by?
    int pos;                          // Position in string (not really)

    for (pos = 0; pos < lim;) {
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
            } else if (is_alpha(c) || c == '<') {
                // VARIABLE?
                token.type = "identifier";
                token.data.push_back("");
                inc = c == '<' ? 1 : 0;
            } else if (c == '/') {
                // ALTERNATIVE
                Token t = {.type = "alternative", .spos = pos, .length = 1};
                tokens.push_back(t);
            } else if (c == '(' || c == ')') {
                // OPEN/CLOSE Sequence group
                Token t = {.type = "sequence_group", .spos = pos, .length = 1};
                t.data.push_back(ctos(c));
                tokens.push_back(t);
            } else if (c == '[' || c == ']') {
                // OPEN/CLOSE Sequence group
                Token t = {.type = "optional_group", .spos = pos, .length = 1};
                t.data.push_back(ctos(c));
                tokens.push_back(t);
            } else if (is_digit(c) || c == '*') {
                token.type = "repetition";
                token.data.push_back(c == '*' ? "0" : "");
                token.spos = pos;
                inc = 0;
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
                    msg = "error: invalid literal for base " + ctos(c);
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
                inc = 0;
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
                    inc = 0;
                }
            }
        } else if (token.type == "identifier") {
            if (is_alpha(c) || c == '_' || c == '-') {
                token.data[0] += c;
            } else {
                if (c == '>') inc++;  // Hop over this
                tokens.push_back(token);
                token = {};
                inc = 0;
            }
        } else if (token.type == "repetition") {
            if (is_digit(c)) {
                *get_last(token.data) += c;
            } else if (c == '*') {
                // Already encountered?
                if (token.data.size() > 1) {
                    msg =
                        "error: unexpected token (already encountered * in "
                        "repetition)";
                    return {pos, 1};
                }
                // Default first arg: 0
                if (token.data[0] == "") token.data[0] = "0";
                token.data.push_back("");
            } else {
                // End number?
                if (token.data.size() == 2 && token.data[1] == "")
                    token.data[1] = std::to_string(ULLONG_MAX);
                // Single-repitition?
                if (token.data.size() == 1)
                    token.data.push_back(
                        std::to_string(std::stoi(token.data[0]) + 1));
                tokens.push_back(token);
                token = {};
                inc = 0;
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

    if (tokens.size() == 0) {
        msg = "error: unexpected end of input";
        return {lim, 2};
    }

    _tokens = tokens;
    return {-1, 0};
}  // namespace abnf

void Variable::lex_fatal() {
    std::string msg;
    int_pair_t val = lex(msg);  // { pos, len } of error
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

// EValuate against given input string
match_return Variable::evaluate(std::string input) {
    return { .ok = false, .msg = "Not Implemented" };
}
};  // namespace abnf