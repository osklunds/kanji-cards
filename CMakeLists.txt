
# General
cmake_minimum_required(VERSION 3.18)
set(CMAKE_CXX_STANDARD 23)
project(KanjiCards)
include(ExternalProject)
set(CMAKE_BUILD_PARALLEL_LEVEL 8)

# Main executable
add_executable(main
src/tests.cpp
src/frequency.cpp
src/frequency_tests.cpp
src/kanji_data.cpp
src/kanji_data_tests.cpp
src/word_data.cpp
src/word_data_tests.cpp
src/stroke_order.cpp
src/stroke_order_tests.cpp
src/util.cpp
src/util_tests.cpp
src/create_card.cpp
src/create_card_tests.cpp
)

# external assets

# Noto Sans
set(NOTO_SANS_URL "https://font.download/dl/font/noto-sans-jp.zip")
set(NOTO_SANS_ARCHIVE ${CMAKE_BINARY_DIR}/noto-sans-jp.zip)
set(NOTO_SANS_EXTRACTED ${CMAKE_BINARY_DIR}/noto-sans-jp)
if (EXISTS ${NOTO_SANS_ARCHIVE})
    message(STATUS "Noto Sans already downloaded, skipping")
else()
    file(DOWNLOAD ${NOTO_SANS_URL} ${NOTO_SANS_ARCHIVE})
    message(STATUS "Noto Sans downloaded")
endif()

if (EXISTS ${NOTO_SANS_EXTRACTED})
    message(STATUS "Noto Sans already extracted, skipping")
else()
    file(ARCHIVE_EXTRACT
         INPUT ${NOTO_SANS_ARCHIVE}
         DESTINATION ${NOTO_SANS_EXTRACTED}
         VERBOSE
        )
    message(STATUS "Noto Sans extracted")
endif()

# # kanjidic2
# execute_process(
#   COMMAND gunzip -c -k ${CMAKE_BINARY_DIR}/../ext_assets/kanjidic2.xml.gz
#   RESULT_VARIABLE result
#   OUTPUT_VARIABLE output
#   OUTPUT_FILE ${CMAKE_BINARY_DIR}/../data/kanjidic2.xml
#   ERROR_VARIABLE error
# )

# if(result)
#   message(FATAL_ERROR "Failed to extract the .gz file. Error: ${error}")
# else()
#   message(STATUS "Extraction successful: ${output}")
# endif()

# # JMdict_e
# execute_process(
#   COMMAND gunzip -c -k ${CMAKE_BINARY_DIR}/../ext_assets/JMdict_e.gz
#   RESULT_VARIABLE result
#   OUTPUT_VARIABLE output
#   OUTPUT_FILE ${CMAKE_BINARY_DIR}/../data/JMdict_e.xml
#   ERROR_VARIABLE error
# )

# if(result)
#   message(FATAL_ERROR "Failed to extract the .gz file. Error: ${error}")
# else()
#   message(STATUS "Extraction successful: ${output}")
# endif()

# # KanjiVG
# if(NOT EXISTS ${CMAKE_BINARY_DIR}/../data)
# file(ARCHIVE_EXTRACT
#      INPUT ${CMAKE_BINARY_DIR}/../ext_assets/kanjivg-20240807-all.zip
#      DESTINATION ${CMAKE_BINARY_DIR}/../data
#      VERBOSE
#     )
#     message(STATUS "KanjiVG extracted")
# else()
#     message(STATUS "KanjiVG already extracted, skipping")
# endif()

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

# todo: download non-repos with cmake, e.g. font
