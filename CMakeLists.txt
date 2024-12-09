cmake_minimum_required(VERSION 3.10)
project(KanjiCards)

# Comment out the below to build the executable aimed for usage
add_definitions(-DTEST)

add_executable(main src/main.cpp
                    src/pugixml.cpp
                    src/tests.cpp
                    src/tests_main.cpp
                    src/frequency.cpp
                    src/frequency_tests.cpp
)
