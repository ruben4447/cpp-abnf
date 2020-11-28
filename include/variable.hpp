#ifndef _VARIABLE_H_
#define _VARIABLE_H_

#include <string>

namespace abnf {
    class Variable {
        private:
            std::string _name;
            std::string _decl_str;

        public:
            std::string get_name();
            std::string get_decl_str();

            void concat_decl(std::string decl_str);

            Variable(std::string name, std::string decl_str);
    };
};

#endif