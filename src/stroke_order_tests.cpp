
// Copyright (C) 2025 Oskar Lundström

// This file is part of kanji-cards.

// kanji-cards is free software: you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free Software
// Foundation, either version 3 of the License, or (at your option) any later
// version.

// kanji-cards is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
// FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
// details.

// You should have received a copy of the GNU General Public License along with
// kanji-cards. If not, see <https://www.gnu.org/licenses/>.

#include "stroke_order.hpp"

#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <regex>
#include <cstdlib>
#include <fstream>
#include "util.hpp"

TEST_CASE("path_for_kanji") {
    std::string path = path_for_kanji("04fd7");
    REQUIRE("kanjivg/kanji/04fd7.svg" == path);
    REQUIRE(std::filesystem::exists(path));
}

TEST_CASE("path_for_kanji_upper_case") {
    std::string path = path_for_kanji("0525D");
    REQUIRE("kanjivg/kanji/0525d.svg" == path);
    REQUIRE(std::filesystem::exists(path));
}

TEST_CASE("find_stroke_nodes") {
    pugi::xml_document doc {};
    pugi::xml_parse_result result = doc.load_file(path_for_kanji("050cd").c_str());
    REQUIRE(result == true);

    auto stroke_nodes = find_stroke_nodes(doc);

    REQUIRE(stroke_nodes.size() == 13);

    auto get_parent_id = [stroke_nodes](auto index) {
        return (std::string)std::get<0>(stroke_nodes[index]).attribute("id").value();
    };
    auto get_child_id = [stroke_nodes](auto index) {
        return (std::string)std::get<1>(stroke_nodes[index]).attribute("id").value();
    };

    REQUIRE(get_child_id(0) == "kvg:050cd-s1");
    REQUIRE(get_child_id(1) == "kvg:050cd-s2");
    REQUIRE(get_child_id(2) == "kvg:050cd-s3");
    REQUIRE(get_child_id(3) == "kvg:050cd-s4");
    REQUIRE(get_child_id(4) == "kvg:050cd-s5");
    REQUIRE(get_child_id(5) == "kvg:050cd-s6");
    REQUIRE(get_child_id(6) == "kvg:050cd-s7");
    REQUIRE(get_child_id(7) == "kvg:050cd-s8");
    REQUIRE(get_child_id(8) == "kvg:050cd-s9");
    REQUIRE(get_child_id(9) == "kvg:050cd-s10");
    REQUIRE(get_child_id(10) == "kvg:050cd-s11");
    REQUIRE(get_child_id(11) == "kvg:050cd-s12");
    REQUIRE(get_child_id(12) == "kvg:050cd-s13");

    REQUIRE(get_parent_id(0) == "kvg:050cd-g1");
    REQUIRE(get_parent_id(1) == "kvg:050cd-g1");
    REQUIRE(get_parent_id(2) == "kvg:050cd-g5");
    REQUIRE(get_parent_id(3) == "kvg:050cd-g6");
    REQUIRE(get_parent_id(4) == "kvg:050cd-g7");
    REQUIRE(get_parent_id(5) == "kvg:050cd-g7");
    REQUIRE(get_parent_id(6) == "kvg:050cd-g7");
    REQUIRE(get_parent_id(7) == "kvg:050cd-g7");
    REQUIRE(get_parent_id(8) == "kvg:050cd-g9");
    REQUIRE(get_parent_id(9) == "kvg:050cd-g7");
    REQUIRE(get_parent_id(10) == "kvg:050cd-g7");
    REQUIRE(get_parent_id(11) == "kvg:050cd-g10");
    REQUIRE(get_parent_id(12) == "kvg:050cd-g10");
}

TEST_CASE("generate_stroke_order_svg_files") {
    std::string path = path_for_kanji("04fd7"); // 俗
    auto svg_files = generate_stroke_order_svg_files(path);
    REQUIRE(svg_files.size() == 9);

    for (int i = 1; i <= 9; i++) {
        auto file = svg_files[i-1];

        // Not a perfect test, but test that some key strings appear
        REQUIRE(file.find("<?xml version=\"1.0\" encoding=\"UTF-8\"?>") !=
                std::string::npos
                );
        REQUIRE(file.find("DOCTYPE") != std::string::npos);

        for (int j = 1; j <= 9; j++) {
            size_t result = file.find("-s" + std::to_string(j));
            if (j <= i) {
                REQUIRE(result != std::string::npos);
            } else {
                REQUIRE(result == std::string::npos);
            }
        }
    }

    // Circles

    // Check all attributes once. Assume they work for the others too. For the
    // others, it's more interesting to check the values that vary.
    REQUIRE(svg_files[0].find("<circle cx=\"30.5\" "
                              "cy=\"17.89\" "
                              "r=\"4\" "
                              "fill=\"red\" "
                              "stroke-width=\"0\"") != std::string::npos);
    REQUIRE(svg_files[1].find("<circle cx=\"24.03\" cy=\"41\"") != std::string::npos);
    REQUIRE(svg_files[2].find("<circle cx=\"52.49\" cy=\"20.23\"") != std::string::npos);
    REQUIRE(svg_files[8].find("<circle cx=\"53.4\" cy=\"90.6\"") != std::string::npos);

    // Grayed out stroke paths

    // For the last file, all paths except the last are grayed out
    REQUIRE( regex_matches(svg_files[8], "-s1[^\n]+stroke=\"rgb\\(180, 180, 180\\)\""));
    REQUIRE( regex_matches(svg_files[8], "-s2[^\n]+stroke"));
    REQUIRE( regex_matches(svg_files[8], "-s3[^\n]+stroke"));
    REQUIRE( regex_matches(svg_files[8], "-s4[^\n]+stroke"));
    REQUIRE( regex_matches(svg_files[8], "-s5[^\n]+stroke"));
    REQUIRE( regex_matches(svg_files[8], "-s6[^\n]+stroke"));
    REQUIRE( regex_matches(svg_files[8], "-s7[^\n]+stroke"));
    REQUIRE( regex_matches(svg_files[8], "-s8[^\n]+stroke"));
    REQUIRE(!regex_matches(svg_files[8], "-s9[^\n]+stroke"));

    // For the second file, only the first path is grayed out
    REQUIRE( regex_matches(svg_files[1], "-s1[^\n]+stroke"));
    REQUIRE(!regex_matches(svg_files[1], "-s2[^\n]+stroke"));
}

// To test that the regex for start position seems to work. These svg files
// have slightly different patterns for the path string.
TEST_CASE("generate_stroke_order_svg_files_regex") {
    auto svg_input_files = {
        "04fd7", // 俗
        "0969b", // 際
        "09803", // 頃
    }; 
    for (auto svg_input_file : svg_input_files) {
        std::string path = path_for_kanji(svg_input_file);
        auto svg_files = generate_stroke_order_svg_files(path);
        REQUIRE(svg_files.size() > 1);
    }
}

TEST_CASE("svg_to_jpg") {
    std::string path = path_for_kanji("04fd7");
    auto svg_files = generate_stroke_order_svg_files(path);
    std::string svg = svg_files[0];
    std::vector<uint8_t> jpg = svg_to_jpg(svg);

    // Some non-perfect checks to see that the conversion seemed to go OK
    REQUIRE(jpg.size() > 500);
    std::string as_string { jpg.begin(), jpg.end() };
    REQUIRE(as_string.find("JFIF") != std::string::npos);
}
