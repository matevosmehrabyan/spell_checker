#include <iostream>
#include <fstream>
#include <string>

#include "InputParser.h"
#include "SpellChecker.h"


int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <filepath>\n";
        return 1;
    }

    std::ifstream input_file(argv[1]);

    if (!input_file) {
        std::cerr << "Failed to open input file!" << std::endl;
        return 1;
    }

    InputParser parser;

    try {
        if (parser.parse(input_file)) {
            SpellChecker spell_checker{parser.get_dictionary()};
            auto corrected_text = spell_checker.spell_check(parser.get_text());
            std::cout << corrected_text << std::endl;
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
