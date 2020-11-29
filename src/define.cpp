#include <regex>
#include <string>

#include "../include/consume.hpp"
#include "../include/utils.hpp"
#include "../include/var_collection.hpp"
#include "../include/variable.hpp"

namespace abnf {
// Define a variable... msg is set to either ERROR or 'variable name'
bool define_var(std::string input, VarCollection* var_collection,
                std::string& msg) {
    int pos = 0;

    std::string original = input;
    consume_whitespace(input);
    pos += original.length() - input.length();

    // Comment?
    if (input[0] == ';') {
        msg = "commented line";
        return true;
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
        return false;
    } else {
        // str(0) contains whole match; str(1) contains in (...)
        var_name = str_lower(matches.str(1));
        pos += matches.str(0).length();
        input.erase(0, matches.str(0).length());
    }

    original = input;
    consume_whitespace(input);
    pos += original.length() - input.length();

    // Extract assignation sign
    pattern = "^=/?";
    std::regex_search(input, matches, pattern);
    std::string assign_op;
    if (matches.size() == 0) {
        msg = "error: expected assignation, got '" + input + "'";
        return false;
    } else {
        assign_op = matches.str(0);
        pos += assign_op.length();
        input.erase(0, assign_op.length());
    }

    // Get variable (var must exist for =/)
    auto var_search = var_collection->vars.find(var_name);
    if (var_search == var_collection->vars.end()) {
        // New variable... create new variable
        Variable var(var_name.c_str(), input, pos);
        var_collection->vars.insert({var_name, var});
    } else if (assign_op == "=") {
        // Overwrite old contents
        var_search->second.def_string = input;
    } else if (assign_op == "=/") {
        // Append input
        var_search->second.def_string += "/ " + input;
    } else {
        msg = "error: invalid assignatopn operator " + input;
        return false;
    }

    msg = var_name;
    return true;
}

// Define variable, but terminate upon error. Return variable name.
std::string define_var_fatal(std::string input, VarCollection* var_collection) {
    std::string msg;
    bool ok = define_var(input, var_collection, msg);
    if (ok)
        return msg;
    else {
        printf("\033[0;31m %s \033[0m \n", msg.c_str());
        exit(1);
    }
}
};  // namespace abnf