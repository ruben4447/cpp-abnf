#include <string>
#include "../include/match_struct.hpp"
#include "../include/token_struct.hpp"
#include "../include/var_collection.hpp"
#include "../include/variable.hpp"
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

    // Evaluate against given input string
    match_return Variable::evaluate(std::string input) {
        std::string text = input;

        int pos = 0;
        int inc = 1;
        bool ok = true;
        std::vector<std::string> matches;
        std::string msg;

        while (ok) {
            Token token = _tokens[pos];
            inc = 1;

            if (token.type == "string") {
                bool str_match = false;
                int len = token.data[1].length();
                std::string cmp = input.substr(0, len);
                if (token.data[0] == "s") str_match = token.data[1] == cmp;
                else str_match = str_lower(token.data[1]) == str_lower(cmp);
                msg = "Match string %" + token.data[0] + "`" + token.data[1] + "` against `" + cmp + "`: " + std::to_string(str_match);

                if (!str_match) {
                    ok = false;
                    break;
                }
            }

            if (inc > 0) pos += inc;
            break;
        }

        match_return value = { .ok = ok, .msg = msg };
        value.matches = matches;
        return value;
    }
};
