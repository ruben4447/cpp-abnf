#include <iostream>
#include <string>

#include "include/var_collection.hpp"
#include "src/define.cpp"

int main() {
    abnf::VarCollection var_collection;
    std::string text = "variable = \"value\"";

    std::string msg;
    bool ok = abnf::define_var(text, &var_collection, msg);

    std::cout << "Success: " << ok << std::endl;
    std::cout << "Msg: " << msg << std::endl;

    return 0;
}