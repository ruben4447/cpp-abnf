#ifndef _PROCESS_FILE_H_
#define _PROCESS_FILE_H_

#include <string>
#include <vector>

#include "var_collection.hpp"

namespace abnf {
void process_file(const char*, VarCollection*);
std::vector<std::string> preprocess_file(std::string file_name);
std::vector<std::string> _preprocess_file(std::string file_name,
                                         std::vector<std::string>& files, std::string include_path);
};  // namespace abnf

#endif