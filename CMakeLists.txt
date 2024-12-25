cmake_minimum_required(VERSION 3.10)
project(KanjiCards)

set(CMAKE_CXX_STANDARD 23)

add_executable(main src/pugixml.cpp
                    src/tests.cpp
                    src/tests_main.cpp
                    src/frequency.cpp
                    src/frequency_tests.cpp
                    src/kanji_data.cpp
                    src/kanji_data_tests.cpp
                    src/word_data.cpp
                    src/word_data_tests.cpp
)
