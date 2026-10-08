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
    static constexpr std::string_view section_separator = "===";
    static constexpr std::size_t max_word_size = 50;

    std::vector<std::string> dictionary;
    std::vector<std::string> text_lines;

    bool parse_section(std::istream& input, std::vector<std::string>& dest, auto handle_line) {
        std::string line;
        bool found_separator{false};

        while (std::getline(input, line)) {
            std::stringstream ss(line);
            std::string cur_word;

            while (ss >> cur_word) {
                if (cur_word == section_separator) {
                    if (line.size() != section_separator.size()) {
                        std::cerr << "Parsing failed! The line should contain "
                                  << "only the section separator" << std::endl;
                        return false;
                    }
                    found_separator = true;
                    break;

                } else if (cur_word.size() > max_word_size || !is_word(cur_word)) {
                    std::cerr << "Parsing failed! The input contains invalid input word '"
                              << cur_word << "'" << std::endl;
                    return false;
                }
            }

            if (found_separator) {
                break;
            }

            handle_line(std::move(line), dest);
        }

        if (!found_separator) {
            std::cerr << "Section separator was not found!" << std::endl;
            return false;
        }

        return true;
    }


public:
    bool parse(std::istream& input) {
        std::vector<std::string> dict_words;
        if (!parse_section(input, dict_words,
                           [](std::string&& line, std::vector<std::string>& dest) {
                               std::stringstream ss(std::move(line));
                               std::string cur_word;
                               while (ss >> cur_word) {
                                   dest.push_back(std::move(cur_word));
                               }
                        }))
        {
            return false;
        }

        std::vector<std::string> parsed_text_lines;
        if (!parse_section(input, parsed_text_lines,
                           [](std::string&& line, std::vector<std::string>& dest) {
                               dest.push_back(std::move(line));
                           }))
        {
            return false;
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
