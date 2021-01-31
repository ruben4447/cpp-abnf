#include <regex>
#include <string>

#include "../include/consume.hpp"
#include "../include/instance.hpp"
#include "../include/utils.hpp"
#include "../include/variable.hpp"

namespace abnf {
// Define a variable... msg is set to either ERROR or 'variable name'. Return
// { latest_pos, length }
int_pair_t Instance::define_var(std::string input, std::string& msg) {
    int pos = 0;

    std::string before = input;
    consume_whitespace(input);
    pos += before.length() - input.length();
    int whitespace_len = pos;

    // Comment?
    if (input[0] == ';') {
        msg = "Line commented out";
        return {-1, 0};
    }

    // Nothing left
    if (input.length() == 0) {
        msg = "End of input";
        return {-1, 0};
    }

    // Extract variable name
    std::string pattern_str = input[0] == '<' ? "^<([A-Za-z][A-Za-z0-9_-]*)>"
                                              : "^([A-Za-z][A-Za-z0-9_-]*)";
    std::regex pattern(pattern_str);
    std::smatch matches;
    std::regex_search(input, matches, pattern);

    std::string var_name;
    if (matches.size() == 0) {
        msg = "error: invalid identifier: expected match of " + pattern_str;
        return {pos, input.length()};
    } else {
        // str(0) contains whole match; str(1) contains in (...)
        var_name = str_lower(matches.str(1));
        int len = matches.str(0).length();
        pos += len;
        input.erase(0, len);
    }

    before = input;
    consume_whitespace(input);
    pos += before.length() - input.length();

    if (input.length() == 0) {
        msg = "error: unexpected end of input";
        return {pos, 2};
    }

    // Extract assignation sign
    pattern = "^(=/|:=|=)";
    std::regex_search(input, matches, pattern);
    std::string assign_op;
    if (matches.size() == 0) {
        msg = "error: expected assignation";
        return {pos, input.length()};
    } else {
        assign_op = matches.str(0);
        pos += assign_op.length();
        input.erase(0, assign_op.length());
    }

    if (input.length() == 0) {
        msg = "error: unexpected end of input";
        return {pos, 2};
    }

    // Get variable (var must exist for =/)
    auto var_search = Instance::vars.find(var_name);
    if (var_search == Instance::vars.end()) {
        if (assign_op == "=/") {
            msg = "error: cannot concatenate to unresolved name " + var_name;
            return {whitespace_len, var_name.length()};
        } else {  // New variable... create new variable
            Variable var(var_name, assign_op == ":=", input);
            Instance::vars.insert({var_name, var});
        }
    } else if (assign_op == ":=") {
        // Cannot use := anywhere but definition
        msg = "error: cannot use constant assignation with resolvable name";
        return {pos - 2, 2};
    } else if (var_search->second.is_const()) {
        msg = "error: mofification of constant name " + var_name;
        int oplen = assign_op.length();
        return {pos - oplen, oplen + input.length()};
    } else if (assign_op == "=") {
        // Overwrite old contents
        var_search->second.def_string = input;
    } else if (assign_op == "=/") {
        // Append input
        var_search->second.def_string += "/ " + input;
    } else {
        msg = "error: unknown assignment operator " + assign_op;
        int len = assign_op.length();
        return {pos - len, len};
    }

    msg = var_name;
    return {-1, 0};
}

// Define variable, but terminate upon error. Return variable name.
std::string Instance::define_var_fatal(std::string input) {
    std::string msg;
    int_pair_t pair = Instance::define_var(input, msg);
    if (pair.first < 0)
        return msg;
    else {
        throw_error(input, msg, pair.first, pair.first + pair.second);
        exit(1);
    }
}
};  // namespace abnf