#include "../include/process_file.hpp"

#include <fstream>
#include <string>

#include "../include/consume.hpp"
#include "../include/utils.hpp"
#include "../include/var_collection.hpp"
#include "../include/define.hpp"

namespace abnf {
void process_file(const char* file_name, VarCollection* var_collection) {
    auto lines = preprocess_file(file_name);
    
    // Define each line as variable
    for (std::string line : lines) {
        define_var_fatal(line, var_collection);
    }
}

std::vector<std::string> preprocess_file(std::string file_name) {
    if (!file_exists(file_name)) throw std::invalid_argument("File " + file_name + " does not exist");
    std::vector<std::string> files;
    return _preprocess_file(file_name, files, get_path(file_name));
}

std::vector<std::string> _preprocess_file(std::string file_name,
                                          std::vector<std::string>& files, std::string include_path) {
    std::vector<std::string> lines;

    std::ifstream fhandle(file_name);
    std::string line;
    while (getline(fhandle, line)) {
        consume_whitespace(line);
        if (line.length() == 0 || line[0] == ';') {
            // Nothing/comment
            continue;
        } else if (line[0] == '#') {
            int dindex = line.find(' ');
            if (dindex == -1) dindex = line.length() - 1; // Set index to EOL
            std::string directive = line.substr(1, dindex - 1);

            // Pre-processor directive
            if (directive == "include") {
                if (line.length() < 9) {
                    throw_error(line, "error: invalid include", 0, line.length());
                    exit(1);
                }
                std::string include = line.substr(9);
                consume_whitespace(include);
                if (include[0] == '<') include = ".\\ABNF\\lib\\" + include.substr(1, include.length() - 2) + ".abnf";
                else include = include_path + "\\" + include;

                if (index_of(files, include) != -1) {
                    throw_error(line, "error: circular include", 9,
                                line.length() - 9);
                    exit(1);
                } else if (!file_exists(include)) {
                    throw_error(line, "File does not exist (full path: '" + include + "')", 9, line.length());
                    exit(1);
                }

                std::vector<std::string> new_files = files;
                new_files.push_back(include);
                auto f_lines = _preprocess_file(include, new_files, include_path);
                for (auto f_line : f_lines) lines.push_back(f_line);
            } else {
                throw_error(line, "error: unknown directive " + directive, 1, dindex);
            }
        } else {
            lines.push_back(line);
        }
    }
    fhandle.close();

    return lines;
}
};  // namespace abnf