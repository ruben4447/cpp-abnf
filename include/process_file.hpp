#ifndef _PROCESS_FILE_H_
#define _PROCESS_FILE_H_

#include <string>
#include <vector>

namespace abnf {
void process_file(const char* file_name);
std::vector<std::string> prepocess_file(std::string file_name);
std::vector<std::string> _prepocess_file(std::string file_name,
                                         std::vector<std::string>& files);
};  // namespace abnf

#endif