#ifndef _INSTANCE_H_
#define _INSTANCE_H_

#include <map>
#include <string>
#include <vector>

#include "variable.hpp"

namespace abnf {
typedef std::vector<std::string> varstack_t;

struct EvalDataStackItem {
    Variable* varptr;
    int input_pos = 0;
    int var_pos = 0;
    int var_len = 0;
};

struct EvalData {
    std::string var;                        // Variable to test
    std::string input;                      // Input to test
    std::string msg;                        // Message
    bool ok = false;                        // Pass?
    int input_pos = 0;                      // Latest input_pos in estack
    std::vector<EvalDataStackItem> estack;  // For error traceback

    EvalData(std::string var, std::string& input) {
        EvalData::var = var;
        EvalData::input = input;
    }
};

class Instance {
   public:
    std::map<std::string, Variable> vars;
    bool feedback = true;  // Feedback from console

    // -- Following functions defined in define.cpp
    int_pair_t define_var(std::string input, std::string& msg);
    std::string define_var_fatal(std::string input);

    // -- Following functions defined in eval.cpp
    bool test(EvalData*);
    bool _test(EvalData*, varstack_t*);

    void load_file(std::string file);
    void execute(std::string input);
};
};  // namespace abnf

#endif