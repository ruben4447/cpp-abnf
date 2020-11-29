#ifndef _VAR_COLLECTION_H_
#define _VAR_COLLECTION_H_

#include <map>
#include <string>

#include "variable.hpp"

namespace abnf {
class VarCollection {
   public:
    std::map<std::string, Variable> vars;
};
};  // namespace abnf

#endif