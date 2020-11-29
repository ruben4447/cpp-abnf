#include "../include/process_file.hpp"

#include <fstream>
#include <string>

#include "../include/consume.hpp"
#include "../include/utils.hpp"

namespace abnf {
void process_file(std::string file_name) { preprocess_file(file_name); }

std::vector<std::string> preprocess_file(std::string file_name) {
    std::vector<std::string> files;
    return _preprocess_file(file_name, files);
}

std::vector<std::string> _preprocess_file(std::string file_name,
                                          std::vector<std::string> files) {
    std::vector<std::string> lines;

    std::ifstream fhandle(file_name);
    std::string line;
    while (getline(fhandle, line)) {
        consume_whitespace(line);
        if (line.length() == 0 || line[0] == ';') {
            // Nothing/comment
            continue;
        } else if (line[0] == '#') {
            // Pre-processor directive
            if (line.substr(0, 8) == "#include") {
                std::string include = line.substr(8);
                printf("Include: %s\n", include.c_str());

                if (index_of(files, include) != -1) {
                    throw_error(line, "error: circular include", 9,
                                line.length() - 9);
                    exit(1);
                }

                std::vector<std::string> new_files = files;
                new_files.push_back(include);
                _preprocess_file(include, files);
            }
        }
    }
    fhandle.close();
}
};  // namespace abnf