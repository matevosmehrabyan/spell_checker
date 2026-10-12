cmake_minimum_required(VERSION 3.20)

project(SpellChecker LANGUAGES CXX)

add_library(spell_checker_core
    src/InputParser.cpp
    src/SpellChecker.cpp
    src/Dictionary.cpp
    src/utils.cpp
)

target_include_directories(spell_checker_core PUBLIC
    "${PROJECT_SOURCE_DIR}/include"
)

target_compile_features(spell_checker_core PUBLIC cxx_std_20)

add_executable(spell_checker
    src/main.cpp
)

target_link_libraries(spell_checker
    PRIVATE spell_checker_core
)

include(CTest)

if(BUILD_TESTING)
    add_executable(test_input_parser
        tests/test_input_parser.cpp
    )

    target_link_libraries(test_input_parser
        PRIVATE spell_checker_core
    )

    add_test(
        NAME input_parser_tests
        COMMAND test_input_parser
    )

    add_executable(test_spell_checker
        tests/test_spell_checker.cpp
    )

    target_link_libraries(test_spell_checker
        PRIVATE spell_checker_core
    )

    add_test(
        NAME spell_checker_tests
        COMMAND test_spell_checker
    )
endif()


install(TARGETS spell_checker
        RUNTIME DESTINATION .
)
