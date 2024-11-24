cmake_minimum_required(VERSION 3.10)
project(KanjiCards)

add_executable(main src/main.cpp
                    src/pugixml.cpp
                    src/tests.cpp
                    src/tests_main.cpp
)
