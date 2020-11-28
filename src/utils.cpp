#ifndef _INCLUDED_UTIL_1
#define _INCLUDED_UTIL_1

#include <string>
#include <fstream>
#include <chrono>
#include <vector>

#include "../include/utils.hpp"

// Given a filename, return contents as std::string
std::string get_file_contents(std::string fileName) {
    std::ifstream file(fileName, std::ios::binary); // Read file buffer
    std::string fileStr; // Initialise string variable
    std::istreambuf_iterator<char> inputIt(file), emptyInputIt; // Read buffer to char type in new variable "emptyInputIt"
    std::back_insert_iterator<std::string> stringInsert(fileStr); // Convert to string
    copy(inputIt, emptyInputIt, stringInsert);
    return fileStr;
}

std::chrono::milliseconds get_time_ms() {
    return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
}

// Split string by delimetter into vector of strings (inefficiently)
std::vector<std::string> string_split(std::string program, char delimiter) {
    std::vector<std::string> lines;
    std::string currString = "";

    for (int i = 0; i < program.length(); i++)
    {
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

// Split string by delimetter into vector of strings (inefficiently)
std::string string_remove(std::string program, char c) {
    std::string output = "";

    for (int i = 0; i < program.length(); i++)
        if (program[i] != c)
            output += program[i];

    return output;
}

std::vector<std::string> string_split_newlines(std::string text) {
    text = string_remove(text, '\r');
    return string_split(text, '\n');
}

void consume_whitespace(std::string& text) {
    while (!text.empty() && (text[0] == ' ' || text[0] == '\t' || text[0] == '\r' || text[0] == '\n')) {
        text.erase(0, 1);
    }
}

#endif