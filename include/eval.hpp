#ifndef _EVAL_H_
#define _EVAL_H_

#include <string>
#include "match_struct.hpp"
#include "var_collection.hpp"

namespace abnf {
    match_return evaluate(VarCollection* var_collection, std::string var, std::string input);
};

#endif