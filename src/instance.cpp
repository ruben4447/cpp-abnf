#include "../include/instance.hpp"

#include <string>

#include "../include/utils.hpp"

namespace abnf {
void Instance::load_file(std::string file) {
    auto lines = file_read_lines(file.c_str());
    for (auto line : lines) {
        Instance::execute(line);
    }
}

// Take input
void Instance::execute(std::string input) {
    consume_whitespace(input);

    // Is a command?
    if (input[0] == ';')
        ;
    else if (input[0] == '!') {
        std::string cmd;
        auto parts = string_split(input, ' ');
        if (parts.size() == 0)
            return print_error(input, "Expected space, found EOL", 1,
                               input.length());
        cmd = parts[0];
        cmd.erase(0, 1);

        if (cmd == "feedback") {
            input.erase(0, 1 + cmd.length());
            consume_whitespace(input);
            if (input == "") {
                printf("Feedback: %s\n", Instance::feedback ? "On" : "Off");
            } else if (input == "on") {
                Instance::feedback = true;
                printf("Feedback: On\n");
            } else if (input == "off") {
                Instance::feedback = false;
            } else {
                print_error("!extrainfo " + input, "Expected boolean on/off",
                            11, 11 + input.length());
            }
        } else if (cmd == "test" || cmd == "testfile") {
            parts = string_split(input, ',');
            if (parts.size() < 2)
                return print_error(input, "Invalid syntax", 1 + cmd.length(),
                                   input.length());
            std::string var = parts[0];
            var.erase(0, 1 + cmd.length());
            consume_whitespace(var);

            std::string to_test = parts[1];
            consume_whitespace(to_test);
            if (cmd == "testfile") {
                if (file_exists(to_test)) {
                    to_test = get_file_contents(to_test);
                } else {
                    return print_error("!testfile ..., " + to_test,
                                       "File does not exist", 15,
                                       15 + to_test.length());
                }
            }

            printf("Testing against variable '%s' against \"%s\"...  ",
                   var.c_str(), to_test.c_str());

            EvalData evaldata(var, to_test);
            Instance::test(&evaldata);
            if (evaldata.ok) {
                evaldata.input.erase(0, evaldata.input_pos);
                printf("\u001b[32mSuccess.\u001b[0m\nRemaining: \"%s\" \n",
                       evaldata.input.c_str());
            } else {
                auto it = Instance::vars.find(var);
                printf("\u001b[31mFail.\u001b[0m\n");
                // Poit=nt error position in input
                print_error(
                    evaldata.input,
                    "Eval Error\n" + evaldata.msg + "\nWhile scanning input",
                    evaldata.input_pos, evaldata.input_pos + 1);

                // Var stack
                for (auto item : evaldata.estack) {
                    print_error(item.varptr->def_string,
                                "In variable <" + item.varptr->get_name() + ">",
                                item.var_pos,
                                item.var_pos + (item.var_len - 1));
                }
            }
        } else if (cmd == "list") {
            int pos = 0;
            int length = Instance::vars.size();
            printf("Variables[%i] { ", length);
            for (auto var : Instance::vars) {
                printf("\"%s\"", var.first.c_str());
                if (pos < length - 1) printf(", ");
                pos++;
            }
            printf(" }\n");
        } else if (cmd == "details") {
            input.erase(0, 1 + cmd.length());
            consume_whitespace(input);

            auto found = Instance::vars.find(str_lower(input));
            if (found == Instance::vars.end()) {
                print_error(input, "Variable does not exist", 0,
                            1 + input.length());
            } else {
                printf("***** Variable %s *****\n",
                       found->second.get_name().c_str());
                found->second.print_tokens();
                printf("\n");
            }
        } else if (cmd == "load") {
            input.erase(0, 5);
            consume_whitespace(input);
            if (input == "<core>") input = "./ABNF/core.abnf";

            if (file_exists(input)) {
                Instance::load_file(input);
                if (Instance::feedback)
                    printf("Loaded file %s\n", input.c_str());
            } else {
                print_error("!load " + input, "File does not exist", 6,
                            6 + input.length());
            }
        } else {
            print_error(input, "Unknown command", 1, 1 + cmd.length());
        }
    } else {
        std::string msg;
        int_pair_t pair = Instance::define_var(input, msg);
        if (pair.first < 0) {
            auto iterator = Instance::vars.find(msg);
            msg = "";
            pair = iterator->second.lex(msg);
            if (pair.first > -1) {
                print_error(iterator->second.def_string, "Lexer Error\n" + msg,
                            pair.first, pair.first + pair.second);
                Instance::vars.erase(iterator);
            } else {
                if (Instance::feedback)
                    printf("Defined variable '%s'\n",
                           iterator->second.get_name().c_str());
            }
        } else
            print_error(input, "Definition Error\n" + msg, pair.first,
                        pair.first + pair.second);
    }
}
};  // namespace abnf