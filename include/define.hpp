#ifndef _DEFINE_H_
#define _DEFINE_H_

#include <string>

#include "var_collection.hpp"
#include "variable.hpp"

namespace abnf {
int_pair_t define_var(std::string input, VarCollection* var_collection,
                      std::string& msg);
std::string define_var_fatal(std::string input, VarCollection* var_collection);
};  // namespace abnf

#endif