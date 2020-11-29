#ifndef _UTILS_H_
#define _UTILS_H_

#include <chrono>
#include <string>
#include <vector>

#include "../include/variable.hpp"

std::string get_file_contents(std::string fileName);
std::vector<std::string> file_read_lines(const char* fileName);
std::string string_remove(std::string program, char c);
std::chrono::milliseconds get_time_ms();
std::vector<std::string> string_split(std::string program, char delimiter);

std::string ctos(const char c);
void _push_token(std::string& type, std::vector<std::string>& data,
                 std::vector<abnf::token_t>& tokens);  // Push to token array

#endif