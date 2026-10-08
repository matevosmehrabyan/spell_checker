#include <iostream>
#include <fstream>
#include <sstream>

#include <vector>
#include <string>
#include <string_view>

#include <algorithm>
#include <cctype>



bool is_word(const std::string& word) {
    return !word.empty() && std::all_of(word.begin(), word.end(), [](unsigned char c) {
                return std::isalpha(c);
            });
}


class InputParser {
    std::vector<std::string> dictionary;
    std::vector<std::string> text_lines;
    static constexpr std::string_view section_separator = "===";

public:
    bool parse(std::istream& input) {
        std::string line;
        std::vector<std::string> dict_words;
        bool found_separator{false};

        while (std::getline(input, line)) {
            std::size_t words_count{0};
            std::stringstream ss(line);
            std::string cur_word;

            while (ss >> cur_word) {
                if (cur_word == section_separator) {
                    found_separator = true;
                } else if (!is_word(cur_word)) {
                    std::cerr << "Parsing failed! The input contains invalid input word '"
                              << cur_word << "'" << std::endl;
                    return false;
                }

                dict_words.push_back(std::move(cur_word));
                ++words_count;
            }

            if (found_separator) {
                if (words_count != 1) {
                    std::cerr << "Invalid input file format. The section separation line "
                              << "should contain only the separator" << std::endl;
                    return false;
                }
                break;
            }
        }

        found_separator = false;
        std::vector<std::string> parsed_text_lines;

        while (std::getline(input, line)) {
            std::size_t words_count{0};
            std::stringstream ss(line);
            std::string cur_word;

            while (ss >> cur_word) {
                if (cur_word == section_separator) {
                    found_separator = true;
                } else if (!is_word(cur_word)) {
                    std::cerr << "Parsing failed! The input contains invalid input word '"
                              << cur_word << "'" << std::endl;
                    return false;
                }

                ++words_count;
            }

            if (found_separator) {
                if (words_count != 1) {
                    std::cerr << "Invalid input file format. The section separation line "
                              << "should contain only the separator" << std::endl;
                    return false;
                }
                break;
            }

            parsed_text_lines.push_back(std::move(line));
        }

        dictionary = std::move(dict_words);
        text_lines = std::move(parsed_text_lines);

        return true;
    }

};



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
