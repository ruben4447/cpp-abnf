#include "../include/var_collection.hpp"

namespace abnf {
    void VarCollection::add_var(Variable& var) {
        auto found = vars.find(var.get_name());
        if (found == vars.end()) {
            // Insert as it doesn't exist in the collection
            vars.insert({ var.get_name(), var });
        } else {
            found->second = var;
        }
    }
};