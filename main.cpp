#include <string>
#include <iostream>

#include "include/var_collection.hpp"
#include "include/match_struct.hpp"

// Include CPP files... when used elsewhere, hpp files are used but there needs
// to be an actual definition, not just a reference
#include "src/consume.cpp"
#include "src/define.cpp"
#include "src/process_file.cpp"
#include "src/utils.cpp"
#include "src/variable.cpp"
#include "src/eval.cpp"

// FORMAT: [abnf_source_file] [comp_file]
int main(int argc, char* argv[]) {
    // int main() {
    abnf::VarCollection var_collection;
    abnf::process_file(argv[1], &var_collection);

    // for (auto item : var_collection.vars) {
    //     printf("***** Variable %s *****\n", item.first.c_str());
    //     item.second.lex_fatal();
    //     item.second.print_tokens();
    //     printf("\n");
    // }

    if (argc > 3) {
        std::string comp_file = get_file_contents(argv[2]);
        
        abnf::match_return val = abnf::evaluate(&var_collection, std::string(argv[3]), comp_file);
        print(&val);
    } else {
        printf("Invalid argv format\nFormat: <abnf_source> <tomatch_source> <tomatch_var>");
    }

    return 0;
}