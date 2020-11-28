#ifndef _VAR_COLLECTION_H_
#define _VAR_COLLECTION_H_

#include <string>
#include <map>

#include "variable.hpp"

namespace abnf {
    class VarCollection {
        public:
            std::map<std::string, Variable> vars;

            void add_var(Variable& var);
    };
};

#endif