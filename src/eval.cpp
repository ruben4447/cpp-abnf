#ifndef _EVAL_H_
#define _EVAL_H_

#include <string>
#include <vector>

#include "../include/instance.hpp"
#include "../include/utils.hpp"

namespace abnf {
bool Instance::test(EvalData* evaldata) {
    varstack_t var_stack = {str_lower(evaldata->var)};
    return Instance::_test(evaldata, &var_stack);
}

bool Instance::_test(EvalData* evaldata, varstack_t* var_stack) {
    evaldata->var = str_lower(evaldata->var);
    std::string var = evaldata->var;

    // TEST IF VARIABLE EXISTS
    auto found = Instance::vars.find(var);
    Variable* var_ptr;
    if (found == Instance::vars.end()) {
        evaldata->msg = "Variable '" + var + "' does not exist in instance";
        return false;
    } else {
        var_ptr = &(found->second);
    }

    int length = evaldata->input.length(), tsize = var_ptr->_tokens.size();
    int var_pos = 0;  // Position of token, NOT same as evaldata->var_pos
    int input_pos = 0;
    char got;  // Current char in unput
    Token* ctoken_ptr;

    while (input_pos < length && var_pos < tsize) {
        printf("pos: %i; var pos: %i / %i\n", input_pos, var_pos, tsize);
        ctoken_ptr = &(var_ptr->_tokens[var_pos]);
        got = evaldata->input[input_pos];
        printf("Type: %s\n", ctoken_ptr->type.c_str());

        if (ctoken_ptr->type == "numeric") {
            // Match all characters
            for (int data_pos = 0; data_pos < ctoken_ptr->data.size();
                 data_pos++, input_pos++) {
                // EOL?
                if (input_pos >= length) {
                    evaldata->msg = "Expected character of code " +
                                    std::string(ctoken_ptr->data[data_pos]) +
                                    ", got EOL";
                    goto test_fail;
                }

                got = evaldata->input[input_pos];
                int expected = atoi(ctoken_ptr->data[data_pos].c_str());
                if (expected == got) {
                    printf("Matched '%c' to numeric '%i'\n", got, expected);
                } else {
                    evaldata->msg = "Expected charcode " +
                                    std::to_string(expected) +
                                    ", got charcode " + std::to_string(got);
                    goto test_fail;
                }
            }
            var_pos++;
        } else if (ctoken_ptr->type == "numeric_range") {
            int range_lower = atoi(ctoken_ptr->data[0].c_str()),
                range_upper = atoi(ctoken_ptr->data[1].c_str());
            if (got >= range_lower && got <= range_upper) {
                printf("Character %c in range %i - %i\n", got, range_lower,
                       range_upper);
            } else {
                evaldata->msg = "Expected character in range " +
                                std::to_string(range_lower) + " - " +
                                std::to_string(range_upper) +
                                ", but got charcode " + std::to_string(got);
                goto test_fail;
            }
            var_pos++;
            input_pos++;
        } else if (ctoken_ptr->type == "string") {
            int l = ctoken_ptr->data[1].length();

            // Too short?
            if (length - input_pos < l) {
                evaldata->msg = "Length of unscanned input is " +
                                std::to_string(length - input_pos) +
                                ", which is less than required string \"" +
                                ctoken_ptr->data[1] + "\"";
                goto test_fail;
            }

            std::string extracted = evaldata->input.substr(input_pos, l);
            bool success;
            if (ctoken_ptr->data[0] == "s") {
                // Case-Sensitive
                success = extracted == ctoken_ptr->data[1];
            } else {
                success =
                    str_lower(extracted) == str_lower(ctoken_ptr->data[1]);
            }

            if (success) {
                printf("String matched: \"%s\" against \"%s\"\n",
                       extracted.c_str(), ctoken_ptr->data[1].c_str());
                input_pos += l;
            } else {
                evaldata->msg = "Expected string \"" + ctoken_ptr->data[1] +
                                "\", got \"" + extracted + "\"";
                goto test_fail;
            }
            var_pos++;
        } else if (ctoken_ptr->type == "identifier") {
            // Cannot loop immediatley to self
            if (var_pos == 0) {
                std::string varref;
                bool cref = ctoken_ptr->data[0] == var_ptr->get_name();
                // printf(
                //     "@ VAR POS 0. Current Var: %s, var going to: %s (cref = "
                //     "%i)\n",
                //     var_ptr->get_name().c_str(), ctoken_ptr->data[0].c_str(),
                //     cref);
                if (!cref) {
                    for (auto var : *var_stack) {
                        if (ctoken_ptr->data[0] == var) {
                            varref = var;
                            cref = true;
                            break;
                        }
                    }
                } else {
                    varref = ctoken_ptr->data[0];
                }
                if (cref) {
                    evaldata->msg = "reference error: circular reference to '" +
                                    varref + "'";
                    goto test_fail;
                }
            }

            // Is variable defined?
            auto it = Instance::vars.find(str_lower(ctoken_ptr->data[0]));
            if (it == Instance::vars.end()) {
                evaldata->msg = "reference error: variable '" +
                                ctoken_ptr->data[0] + "' does not exist";
                goto test_fail;
            } else {
                std::string new_input = evaldata->input;
                EvalData edata(ctoken_ptr->data[0], new_input);
                edata.input.erase(0, input_pos);
                printf("Pass Input: '%s'\n", edata.input.c_str());

                var_stack->push_back(str_lower(ctoken_ptr->data[0]));
                bool okay = Instance::_test(&edata, var_stack);
                input_pos += edata.input_pos;
                if (!okay) {
                    for (auto item : edata.estack)
                        evaldata->estack.push_back(item);
                    evaldata->msg = edata.msg;
                    goto test_fail;
                }
                var_stack->pop_back();
            }
            var_pos++;
        } else {
            evaldata->msg = "error: unknown token type: " + ctoken_ptr->type;
            goto test_fail;
        }
    }

    // Have all tokens been scanned?
    if (var_pos < var_ptr->_tokens.size()) {
        evaldata->msg = "Unexpected EOL while scanning";
        goto test_fail;
    }

    goto test_success;
test_fail:
    evaldata->estack.push_back({.varptr = var_ptr,
                                .input_pos = input_pos,
                                .var_pos = ctoken_ptr->spos,
                                .var_len = ctoken_ptr->length});
    evaldata->input_pos = input_pos;
    evaldata->ok = false;
    return false;

test_success:
    evaldata->input_pos = input_pos;
    evaldata->ok = true;
    return true;
}
};  // namespace abnf

#endif