#ifndef _INCLUDED_UTIL_1
#define _INCLUDED_UTIL_1

#define CASE_TRANSFORM 'a' - 'A'

#include "../include/utils.hpp"

#include <chrono>
#include <fstream>
#include <string>
#include <vector>
#include <sys/stat.h>

// Given a filename, return contents as std::string
std::string get_file_contents(std::string fileName) {
    std::string contents, line;
    std::ifstream file(fileName);
    while (getline(file, line)) {
        contents += line + '\n';
    }
    file.close();
    return contents;
}

// Get lines from a file
std::vector<std::string> file_read_lines(const char* fileName) {
    std::ifstream file(fileName);
    std::vector<std::string> lines;
    std::string line;
    while (getline(file, line)) lines.push_back(line);
    file.close();
    return lines;
}

std::chrono::milliseconds get_time_ms() {
    return std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch());
}

// Split string by delimetter into vector of strings (inefficiently)
std::vector<std::string> string_split(std::string program, char delimiter) {
    std::vector<std::string> lines;
    std::string currString = "";

    for (int i = 0; i < program.length(); i++) {
        if (program[i] == delimiter) {
            lines.push_back(currString);
            currString = "";
        } else {
            currString += program[i];
        }
    }
    if (currString != "") lines.push_back(currString);

    return lines;
}

// Remove char from string
std::string string_remove(std::string program, char c) {
    std::string output = "";

    for (int i = 0; i < program.length(); i++)
        if (program[i] != c) output += program[i];

    return output;
}

bool is_digit(char c) { return c >= '0' && c <= '9'; }

// Does the given digit match the given base?
bool digit_match_base(char base, char digit) {
    return (base == 'b' && (digit == '0' || digit == '1')) ||
           (base == 'd' && is_digit(digit)) ||
           (base == 'x' && (is_digit(digit) || (digit >= 'a' && digit <= 'f') ||
                            (digit >= 'A' && digit <= 'F')));
}

// Convert base char to int
int charbase_to_int(char base) {
    if (base == 'd') return 10;
    if (base == 'b') return 2;
    if (base == 'x') return 16;
    throw std::runtime_error("Invalid char base: `" + ctos(base) + "`");
    exit(1);
}

// Is alpha? [A-Za-z]
bool is_alpha(char c) {
    return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z');
}

// Convert string to lower case
std::string str_lower(const std::string str) {
    std::string out;
    for (char c : str) out += (c >= 'A' && c <= 'Z') ? c + CASE_TRANSFORM : c;
    return out;
}

// Return pointer to last string in vector
std::string* get_last(std::vector<std::string>& vector) {
    return &(vector[vector.size() - 1]);
}

// Convert character to string
std::string ctos(const char c) { return std::string(1, c); }

// Get index of thing in vector
int index_of(std::vector<std::string>& strings, std::string& string) {
    int length = strings.size();
    for (int i = 0; i < length; ++i) {
        if (string == strings[i]) return i;
    }
    return -1;
}

// Throw an error
void throw_error(std::string text, std::string error, int pos_start,
                 int pos_end = -1) {
    int length = pos_end - pos_start;
    if (length < 0) length = 1;

    // Pad with spaces
    while (text.length() < pos_end) text += ' ';

    // Print error
    printf("\033[1;31m%s\033[0m\n", error.c_str());

    // Highlight error'd bit
    std::string norm = "\033[40m\033[37;1m";
    printf("%s%s\033[0m", norm.c_str(), text.substr(0, pos_start).c_str());
    printf("\033[37;1m\033[41m%s\033[0m",
           text.substr(pos_start, length).c_str());
    printf("%s%s\033[0m", norm.c_str(), text.substr(pos_end).c_str());

    // Print pointer to error
    printf("\n%s", norm.c_str());
    for (int i = 0; i < pos_start; i++) printf(" ");
    printf("\033[1;31m^");
    for (int i = 1; i < length; i++) printf("~");
    printf("\033[0m\n");

    exit(1);
}

// Does the given file name exist?
bool file_exists(const std::string &name) {
    struct stat buffer;
    return (stat(name.c_str(), &buffer) == 0);
}

std::string get_path(const std::string &fname) {
    size_t pos = fname.find_last_of("\\/");
    return (std::string::npos == pos)
               ? ""
               : fname.substr(0, pos);
}

#endif