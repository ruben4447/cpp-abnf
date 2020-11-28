#ifndef _DEFINE_H_
#define _DEFINE_H_

#include <string>
#include "var_collection.hpp"

namespace abnf {
    bool define_var(std::string input, VarCollection* var_collection, std::string& msg);
};

#endif