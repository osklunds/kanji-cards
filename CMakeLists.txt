cmake_minimum_required(VERSION 3.10)
project(KanjiCards)

set(CMAKE_CXX_STANDARD 23)

set(PUGIXML_DIR ${CMAKE_BINARY_DIR}/pugixml)

include(ExternalProject)
ExternalProject_add(pugixml
  GIT_REPOSITORY https://github.com/zeux/pugixml.git
  GIT_TAG 4bc1441
  PREFIX ${CMAKE_BINARY_DIR}/_deps
  CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${PUGIXML_DIR}
)

include_directories(${PUGIXML_DIR}/include)

add_executable(main 
                    src/tests.cpp
                    src/tests_main.cpp
                    src/frequency.cpp
                    src/frequency_tests.cpp
                    src/kanji_data.cpp
                    src/kanji_data_tests.cpp
                    src/word_data.cpp
                    src/word_data_tests.cpp
)

add_dependencies(main pugixml)
target_link_libraries(main ${PUGIXML_DIR}/lib64/libpugixml.a)
