#include "Dictionary.h"
#include "InputParser.h"   // Provides Chunk / ChunkType in the current project layout.
#include "SpellChecker.h"

#include <cstddef>
#include <iostream>
#include <iterator>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

namespace {

void check(bool condition, std::string_view name, int& failures)
{
    if (condition) {
        std::cout << "[PASS] " << name << '\n';
    } else {
        std::cerr << "[FAIL] " << name << '\n';
        ++failures;
    }
}

Chunk word_chunk(std::string content)
{
    return Chunk(std::move(content), ChunkType::WORD);
}

Chunk whitespace_chunk(std::string content)
{
    return Chunk(std::move(content), ChunkType::WS);
}

std::string run_spell_checker(const std::vector<std::string>& dictionary_words,
                              const std::vector<Chunk>& chunks)
{
    Dictionary dictionary;
    for (const auto& word : dictionary_words) {
        dictionary.add(word);
    }

    SpellChecker checker(dictionary);
    return checker.spell_check(chunks);
}

} // namespace

int main()
{
    int failures{0};

    check(run_spell_checker({"Hello", "world"},
                            {word_chunk("hELLo"), whitespace_chunk(" "), word_chunk("WORLD")})
              == "hELLo WORLD",
          "case-insensitive exact matches preserve text spelling", failures);

    check(run_spell_checker({"cat"}, {word_chunk("cats")}) == "cat",
          "one-edit correction is emitted without braces", failures);

    check(run_spell_checker({"scat", "cats", "cart"}, {word_chunk("cat")})
              == "{scat cats cart}",
          "all one-edit corrections preserve dictionary order and formatting", failures);

    check(run_spell_checker({"bat", "cats"}, {word_chunk("cat")}) == "cats",
          "one-edit corrections take priority over two-edit corrections", failures);

    check(run_spell_checker({"cut", "bat"}, {word_chunk("cat")}) == "{cut bat}",
          "two-edit corrections are returned in dictionary order when no one-edit match exists",
          failures);

    check(run_spell_checker({"the"}, {word_chunk("teh")}) == "the",
          "two edits can correct a transposition when permitted", failures);

    check(run_spell_checker({"cat"}, {word_chunk("unrelated")}) == "{unrelated?}",
          "unfixable word is marked with braces and question mark", failures);

    check(run_spell_checker({"abc"}, {word_chunk("bc")}) == "abc",
          "one insertion at position zero is found", failures);

    check(run_spell_checker({"bab"}, {word_chunk("a")}) == "bab",
          "same-type insertions separated by a matched character are allowed", failures);

    check(run_spell_checker({"abb"}, {word_chunk("a")}) == "{a?}",
          "two adjacent insertions are rejected", failures);

    check(run_spell_checker({"a"}, {word_chunk("abb")}) == "{abb?}",
          "two adjacent deletions are rejected", failures);

    // These are important regression tests for the alignment ambiguity discussed during
    // implementation. The two inserted/deleted characters can be separated by the
    // original/matched middle character, so these transformations should be accepted.
    // The current greedy matching loop is expected to fail these until it explores
    // alternative alignments instead of committing to the first equal character.
    check(run_spell_checker({"aaa"}, {word_chunk("a")}) == "aaa",
          "two non-adjacent insertions around the original character (a -> aaa)", failures);

    check(run_spell_checker({"a"}, {word_chunk("aaa")}) == "a",
          "two non-adjacent deletions around the retained middle character (aaa -> a)", failures);

    check(run_spell_checker({"House"}, {word_chunk("houes")}) == "House",
          "correction uses original dictionary capitalization", failures);

    check(run_spell_checker({"the", "cat"},
                            {whitespace_chunk(" \t"), word_chunk("teh"),
                             whitespace_chunk("  \n"), word_chunk("cat"),
                             whitespace_chunk("\n")})
              == " \tthe  \ncat\n",
          "whitespace chunks are preserved exactly", failures);

    check(run_spell_checker({"cat"}, {}) == "",
          "empty text produces empty output", failures);

    // Duplicate policy for this project: every normalized vector entry remains in
    // iteration order, while the lowercase->original map keeps the first spelling.
    {
        Dictionary dictionary;
        dictionary.add("Apple");
        dictionary.add("APPLE");

        check(dictionary.contains("apple"), "dictionary lookup is case-insensitive", failures);
        check(dictionary.get("apple") == "Apple",
              "duplicate normalized key retains the first original spelling", failures);
        check(std::distance(dictionary.begin(), dictionary.end()) == 2,
              "duplicate dictionary entries remain in dictionary iteration order", failures);
    }

    check(run_spell_checker({"Apple", "APPLE"}, {word_chunk("appl")}) == "{Apple Apple}",
          "duplicate normalized dictionary entries produce duplicate suggestions in order",
          failures);

    // Exercise output building over a much longer input without triggering correction
    // searches for each token.
    {
        Dictionary dictionary;
        dictionary.add("hello");
        SpellChecker checker(dictionary);

        constexpr std::size_t word_count = 3000;
        std::vector<Chunk> chunks;
        chunks.reserve(word_count * 2 - 1);

        std::string expected;
        expected.reserve(word_count * 6 - 1);
        for (std::size_t i = 0; i < word_count; ++i) {
            if (i != 0) {
                chunks.push_back(whitespace_chunk(" "));
                expected.push_back(' ');
            }
            chunks.push_back(word_chunk("hello"));
            expected += "hello";
        }

        check(checker.spell_check(chunks) == expected,
              "large text input preserves all words and separators", failures);
    }

    if (failures != 0) {
        std::cerr << failures << " spell-checker test(s) failed\n";
        return 1;
    }

    std::cout << "All spell-checker tests passed\n";
    return 0;
}
