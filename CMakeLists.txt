
# General
cmake_minimum_required(VERSION 3.18)
set(CMAKE_CXX_STANDARD 23)
project(KanjiCards)
include(ExternalProject)
set(CMAKE_BUILD_PARALLEL_LEVEL 8)
set(CMAKE_CXX_FLAGS_RELEASE "-O3")
set(CMAKE_CXX_FLAGS "-Wall -Wextra -Werror")

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

function(download_if_needed url local_path)
    if (EXISTS ${local_path})
        message(STATUS "'${url}' already downloaded, skipping")
    else()
        file(DOWNLOAD ${url} ${local_path})
        message(STATUS "'${url}' downloaded")
    endif()
endfunction()

function(extract_if_needed archive_path extract_path)
    if (EXISTS ${extract_path})
        message(STATUS "'${archive_path}' already extracted, skipping")
    else()
        file(ARCHIVE_EXTRACT
             INPUT ${archive_path}
             DESTINATION ${extract_path}
             VERBOSE
            )
        message(STATUS "'${archive_path}' extracted")
    endif()
endfunction()

function(unzip_if_needed archive_path extract_path)
    if (EXISTS ${extract_path})
        message(STATUS "'${archive_path}' already unzipped, skipping")
    else()
        execute_process(
            COMMAND gunzip -c -k ${archive_path}
            RESULT_VARIABLE result
            OUTPUT_FILE ${extract_path}
            ERROR_VARIABLE error
        )
        if(result)
            message(FATAL_ERROR "Failed to unzip '${archive_path}'. Error: ${error}")
        endif()
        message(STATUS "'${archive_path}' unzipped")
    endif()
endfunction()

set(EXT_ASSETS_DIR ${CMAKE_BINARY_DIR}/../ext_assets)

# noto-sans-jp
set(NOTO_SANS_JP_ARCHIVE ${EXT_ASSETS_DIR}/Noto_Sans_JP.zip)
set(NOTO_SANS_JP_EXTRACTED ${CMAKE_BINARY_DIR}/noto-sans-jp)
extract_if_needed(${NOTO_SANS_JP_ARCHIVE} ${NOTO_SANS_JP_EXTRACTED})

# kanjidic2
set(KANJIDIC2_ARCHIVE ${EXT_ASSETS_DIR}/kanjidic2.xml.gz)
set(KANJIDIC2_EXTARCTED ${CMAKE_BINARY_DIR}/kanjidic2.xml)
unzip_if_needed(${KANJIDIC2_ARCHIVE} ${KANJIDIC2_EXTARCTED})

# JMdict_e
set(JMDICT_E_ARCHIVE ${EXT_ASSETS_DIR}/JMdict_e.gz)
set(JMDICT_E_EXTARCTED ${CMAKE_BINARY_DIR}/JMdict_e.xml)
unzip_if_needed(${JMDICT_E_ARCHIVE} ${JMDICT_E_EXTARCTED})

execute_process(
    COMMAND ${CMAKE_SOURCE_DIR}/src/create_JMdict_e.xml.sample.sh
    RESULT_VARIABLE result
    ERROR_VARIABLE error
)
if(result)
    message(FATAL_ERROR "Failed to create JMdict_e.xml.sample. Error: ${error}")
endif()
message(STATUS "JMdict_e.xml.sample created")

# kanjivg
set(KANJIVG_ARCHIVE ${EXT_ASSETS_DIR}/kanjivg-20240807-all.zip)
set(KANJIVG_EXTRACTED ${CMAKE_BINARY_DIR}/kanjivg)
extract_if_needed(${KANJIVG_ARCHIVE} ${KANJIVG_EXTRACTED})

# Kanji frequency list
set(KANJI_FREQUENCY_LIST_URL "https://docs.google.com/spreadsheets/d/1MBYfKPrlST3F51KIKbAlsGw1x4c_atuHfPwSSRN5sLs/export?gid=496425456&format=csv")
set(KANJI_FREQUENCY_LIST_FILE ${CMAKE_BINARY_DIR}/kanji_frequency_list.csv)
download_if_needed(${KANJI_FREQUENCY_LIST_URL} ${KANJI_FREQUENCY_LIST_FILE})

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
