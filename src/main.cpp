#include <iostream>
#include <fstream>
#include <string>
#include <stdexcept>

#include "InputParser.h"
#include "SpellChecker.h"


int main(int argc, char* argv[]) {
    InputParser parser;

    try {
        if (argc > 1) {
            std::ifstream input_file(argv[1]);
            if (!input_file) {
                std::cerr << "Failed to open input file!" << std::endl;
                return 1;
            }
            parser.parse(input_file);
        } else{
            parser.parse(std::cin);
        }

        SpellChecker spell_checker;
        auto corrected_text = spell_checker.spell_check(parser.get_dictionary(), parser.get_text());
        std::cout << corrected_text;

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
