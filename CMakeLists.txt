cmake_minimum_required(VERSION 3.20)

project(SpellChecker LANGUAGES CXX)

add_executable(spell_checker
    src/main.cpp
    src/InputParser.cpp
    src/SpellCheck.cpp
    src/Dictionary.cpp
    src/utils.cpp
)

install(TARGETS spell_checker
        RUNTIME DESTINATION .
)

target_include_directories(spell_checker PRIVATE
    ${PROJECT_SOURCE_DIR}/include
)

target_compile_features(spell_checker PRIVATE cxx_std_20)
