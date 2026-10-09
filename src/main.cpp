#include <iostream>
#include <fstream>
#include <string>

#include "InputParser.h"


int main() {
    std::string input_file_path("input.txt");
    std::ifstream input_file(input_file_path.c_str());

    if (!input_file) {
        std::cerr << "Failed to open input file!" << std::endl;
        return 1;
    }

    InputParser parser;

    try {
        if (parser.parse(input_file)) {
            std::cout << "Parse successful!" << std::endl;
        } else {
            std::cerr << "Parsing failed. Please, check if the input file is in valid format." << std::endl;
            return 1;
        }
    } catch (...) {
        std::cerr << "Something unexpected happened!" << std::endl;
        return 1;
    }

    return 0;
}
