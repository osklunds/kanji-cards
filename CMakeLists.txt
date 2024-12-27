
# General
cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 23)
project(KanjiCards)
include(ExternalProject)

# Main executable
add_executable(main
src/tests.cpp
src/frequency.cpp
src/frequency_tests.cpp
src/kanji_data.cpp
src/kanji_data_tests.cpp
src/word_data.cpp
src/word_data_tests.cpp
)

# pugixml
set(PUGIXML_DIR ${CMAKE_BINARY_DIR}/pugixml)

ExternalProject_add(pugixml
  GIT_REPOSITORY https://github.com/zeux/pugixml.git
  GIT_TAG 4bc1441
  PREFIX ${CMAKE_BINARY_DIR}/pugixml_git
  CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${PUGIXML_DIR}
)

include_directories(${PUGIXML_DIR}/include)
target_link_libraries(main ${PUGIXML_DIR}/lib64/libpugixml.a)
add_dependencies(main pugixml)

# catch2
set(CATCH2_DIR ${CMAKE_BINARY_DIR}/catch2)

ExternalProject_add(catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2
  GIT_TAG 0321d2f
  PREFIX ${CMAKE_BINARY_DIR}/catch2_git
  CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${CATCH2_DIR}
)

include_directories(${CATCH2_DIR}/include)
target_link_libraries(main ${CATCH2_DIR}/lib64/libCatch2Main.a)
target_link_libraries(main ${CATCH2_DIR}/lib64/libCatch2.a)

add_dependencies(main catch2)

# libharu

set(LIBHARU_DIR ${CMAKE_BINARY_DIR}/libharu)

ExternalProject_add(libharu
  GIT_REPOSITORY https://github.com/libharu/libharu.git
  GIT_TAG 0c598be
  PREFIX ${CMAKE_BINARY_DIR}/libharu_git
  CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${LIBHARU_DIR}
)

include_directories(${LIBHARU_DIR}/include)
target_link_libraries(main ${LIBHARU_DIR}/lib64/libhpdf.so)

add_dependencies(main libharu)
