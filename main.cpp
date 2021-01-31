#include <string>

#include "include/instance.hpp"

// Include CPP files... when used elsewhere, hpp files are used but there needs
// to be an actual definition, not just a reference
#include "src/consume.cpp"   // Consuming input (used in definition)
#include "src/define.cpp"    // Defining a variable
#include "src/eval.cpp"      // Evaluating variables against input
#include "src/instance.cpp"  // Wrapper class for containing variables
#include "src/lex.cpp"       // For lexing variables (post-definition)
#include "src/utils.cpp"     // Utility things
#include "src/variable.cpp"  // Variable class

int main(int argc, char* argv[]) {
    // int main() {
    abnf::Instance instance;
    instance.execute("!load " + std::string(argv[1]));
    // instance.execute("!load .\\ABNF\\tests\\eval\\or.abnf");
    return 0;
}
