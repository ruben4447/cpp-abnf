#include <string>
#include "../include/match_struct.hpp"
#include "../include/var_collection.hpp"
#include "../include/eval.hpp"

namespace abnf {
    // Evaluate input against a variable
    match_return evaluate(VarCollection* var_collection, std::string var, std::string input) {
        auto found = var_collection->vars.find(var);
        if (found == var_collection->vars.end()) {
            match_return value;
            value.ok = false;
            value.msg = "error: Variable '" + var + "' not found";
            return value;
        } else {
            return found->second.evaluate(input);
        }
    }
};
