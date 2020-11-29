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

bool is_digit(char c);
bool digit_match_base(char base, char digit);
int charbase_to_int(char base);
bool is_alpha(char c);
std::string str_lower(const std::string str);
std::string* get_last(std::vector<std::string>& vector);

std::string ctos(const char c);
int index_of(std::vector<std::string>&, std::string&);

void throw_error(std::string text, std::string error, int pos_start,
                 int pos_end);

#endif