#include <iostream>
#include <string>

#include "include/var_collection.hpp"

// Include CPP files... when used elsewhere, hpp files are used but there needs
// to be an actual definition, not just a reference
#include "src/define.cpp"
#include "src/utils.cpp"
#include "src/variable.cpp"

int main(int argc, char* argv[]) {
    // int main() {
    abnf::VarCollection var_collection;

    std::string src_file(argv[1]);
    std::string contents = get_file_contents(src_file);
    auto lines = string_split_newlines(contents);

    std::string msg;
    bool ok = abnf::define_var(lines[0], &var_collection, msg);

    std::cout << "Success: " << ok << std::endl;
    std::cout << "Msg: " << msg << std::endl;

    return 0;
}