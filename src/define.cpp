#include <string>
#include <regex>

#include "../include/var_collection.hpp"

namespace abnf {
    // Define a variable
    bool define_var(std::string input, VarCollection* var_collection, std::string &msg) {
        // Extract variable name
        std::regex pattern("^<?([A-Za-z][A-Za-z0-9_-]*)>?");
        std::smatch matches;
        std::regex_search(input, matches, pattern);

        if (matches.size() == 0) {
            msg = "error: invalid identifier / variable name";
            return false;
        } else {
            input.erase(0, matches.str(0).length())
        }
        std::string var_name = matches.str(0);

        
        return true;
    }
};