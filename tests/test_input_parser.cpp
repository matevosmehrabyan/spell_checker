#include "InputParser.h"

#include <iostream>
#include <sstream>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

namespace {

struct TestCase {
    std::string_view name;
    std::string input;
    bool expected_success;
};

void check(bool condition, std::string_view name, int& failures)
{
    if (condition) {
        std::cout << "[PASS] " << name << '\n';
    } else {
        std::cerr << "[FAIL] " << name << '\n';
        ++failures;
    }
}

bool parse_input(const std::string& contents)
{
    InputParser parser;
    std::istringstream input{contents};
    return parser.parse(input);
}

std::string make_alpha_word(std::size_t value)
{
    std::string suffix;
    do {
        suffix.push_back(static_cast<char>('a' + value % 26));
        value /= 26;
    } while (value != 0);

    std::string word{"entry"};
    for (auto it = suffix.rbegin(); it != suffix.rend(); ++it) {
        word.push_back(*it);
    }
    return word;
}

} // namespace

int main()
{
    int failures{0};

    const std::string word_50(50, 'a');
    const std::string word_51(51, 'a');

    const std::vector<TestCase> cases{
        {"normal dictionary and text", "cat\ndog\n===\ncat dog\n===", true},
        {"empty input is rejected", "", false},
        {"dictionary without a separator is rejected", "cat\ndog\n", false},
        {"no separator is rejected", "cat\ndog\nhello\n", false},
        {"only one separator is rejected (text terminator missing)", "===\n", false},
        {"empty dictionary and empty text are accepted", "===\n===", true},
        {"empty dictionary with nonempty text is accepted", "===\nhello\n===", true},
        {"nonempty dictionary with empty text is accepted", "cat\n===\n===", true},
        {"closing separator without final newline is accepted", "cat\n===\ncat\n===", true},
        {"blank and whitespace-only text lines are accepted",
         "cat\n===\n\n\t \ncat\n===", true},
        {"leading and trailing text whitespace is accepted",
         "cat\n===\n  cat \t\n===", true},
        {"malformed separator with leading whitespace in dictionary is rejected",
         "cat\n ===\n===\ntext\n===", false},
        {"malformed separator with trailing whitespace in dictionary is rejected",
         "cat\n=== \n===\ntext\n===", false},
        {"word and separator on one dictionary line are rejected",
         "cat ===\n===\ntext\n===", false},
        {"malformed separator with leading whitespace in text is rejected",
         "cat\n===\n ===\n===", false},
        {"malformed separator with trailing whitespace in text is rejected",
         "cat\n===\n=== \n===", false},
        {"word and separator on one text line are rejected",
         "cat\n===\nhello ===\n===", false},
        {"invalid punctuation in dictionary word is rejected",
         "bad-word\n===\ntext\n===", false},
        {"invalid punctuation in text word is rejected",
         "cat\n===\nhello!\n===", false},
        {"dictionary word of length 50 is accepted",
         word_50 + "\n===\n===", true},
        {"dictionary word of length 51 is rejected",
         word_51 + "\n===\n===", false},
        {"text word of length 50 is accepted",
         "cat\n===\n" + word_50 + "\n===", true},
        {"text word of length 51 is rejected",
         "cat\n===\n" + word_51 + "\n===", false},
        {"missing text-section terminator is rejected",
         "cat\n===\nhello", false},
    };

    for (const auto& test : cases) {
        check(parse_input(test.input) == test.expected_success, test.name, failures);
    }

    // A larger dictionary catches accidental assumptions about tiny input.
    {
        std::string input_text;
        constexpr std::size_t dictionary_size = 2500;
        for (std::size_t i = 0; i < dictionary_size; ++i) {
            input_text += make_alpha_word(i);
            input_text += '\n';
        }
        input_text += "===\n";
        input_text += make_alpha_word(1234);
        input_text += "\n===";

        check(parse_input(input_text), "large dictionary parses successfully", failures);
    }

    if (failures != 0) {
        std::cerr << failures << " input-parser test(s) failed\n";
        return 1;
    }

    std::cout << "All input-parser tests passed\n";
    return 0;
}
