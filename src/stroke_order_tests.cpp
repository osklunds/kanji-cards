
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
    REQUIRE("../data/kanji/04fd7.svg" == path);
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

    // todo: delete file first, and check that check fails
    std::ofstream out_file {};
    out_file.open ("out.jpg");
    for (auto byte : jpg) {
        out_file << byte;
    }
    out_file.close();
}

// for the found stroke nodes, delete them one by one, starting with the latest
// and write the svg. Then insert the DTD.

// cat 04fd7-4.svg | magick svg:- out.jpg
