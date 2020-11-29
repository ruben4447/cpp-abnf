#ifndef _INCLUDED_UTIL_1
#define _INCLUDED_UTIL_1

#include "../include/utils.hpp"

#include <chrono>
#include <fstream>
#include <string>
#include <vector>

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

// Convert character to string
std::string ctos(const char c) { return std::string(1, c); }

void _push_token(std::string& type, std::vector<std::string>& data,
                 std::vector<abnf::token_t>& tokens) {
    tokens.push_back({type, data});
    type = "";
    data.clear();
}

#endif