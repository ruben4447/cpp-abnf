#ifndef _UTILS_H_
#define _UTILS_H_

#include <string>
#include <chrono>
#include <vector>

std::string get_file_contents(std::string fileName);
std::string string_remove(std::string program, char c);
std::chrono::milliseconds get_time_ms();
std::vector<std::string> string_split(std::string program, char delimiter);
void consume_whitespace(std::string &text);

#endif