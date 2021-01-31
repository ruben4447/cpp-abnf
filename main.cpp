#include <string>

#include "include/instance.hpp"

// Include CPP files... when used elsewhere, hpp files are used but there needs
// to be an actual definition, not just a reference
#include "src/consume.cpp"
#include "src/define.cpp"
#include "src/eval.cpp"
#include "src/instance.cpp"
#include "src/utils.cpp"
#include "src/variable.cpp"

int main(int argc, char* argv[]) {
    abnf::Instance instance;
    instance.execute("!load " + std::string(argv[1]));
    return 0;
}