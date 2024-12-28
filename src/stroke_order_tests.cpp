
#include "stroke_order.hpp"

#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <regex>

TEST_CASE("path_for_kanji") {
    std::string path = path_for_kanji("04fd7");
    REQUIRE("../data/kanji/04fd7.svg" == path);
    REQUIRE(std::filesystem::exists(path));
}

TEST_CASE("find_stroke_nodes") {
    pugi::xml_document doc {};
    pugi::xml_parse_result result = doc.load_file(path_for_kanji("04fd7").c_str());
    REQUIRE(result == true);

    auto stroke_nodes = find_stroke_nodes(doc);

    REQUIRE(stroke_nodes.size() == 9);

    auto get_parent_id = [stroke_nodes](auto index) {
        return (std::string)std::get<0>(stroke_nodes[index]).attribute("id").value();
    };
    auto get_child_id = [stroke_nodes](auto index) {
        return (std::string)std::get<1>(stroke_nodes[index]).attribute("id").value();
    };

    REQUIRE(get_parent_id(0) == "kvg:04fd7-g1");
    REQUIRE(get_child_id(0) == "kvg:04fd7-s1");

    REQUIRE(get_parent_id(1) == "kvg:04fd7-g1");
    REQUIRE(get_child_id(1) == "kvg:04fd7-s2");

    REQUIRE(get_parent_id(2) == "kvg:04fd7-g3");
    REQUIRE(get_child_id(2) == "kvg:04fd7-s3");

    REQUIRE(get_parent_id(3) == "kvg:04fd7-g3");
    REQUIRE(get_child_id(3) == "kvg:04fd7-s4");

    REQUIRE(get_parent_id(4) == "kvg:04fd7-g4");
    REQUIRE(get_child_id(4) == "kvg:04fd7-s5");

    REQUIRE(get_parent_id(5) == "kvg:04fd7-g4");
    REQUIRE(get_child_id(5) == "kvg:04fd7-s6");

    REQUIRE(get_parent_id(6) == "kvg:04fd7-g5");
    REQUIRE(get_child_id(6) == "kvg:04fd7-s7");

    REQUIRE(get_parent_id(7) == "kvg:04fd7-g5");
    REQUIRE(get_child_id(7) == "kvg:04fd7-s8");

    REQUIRE(get_parent_id(8) == "kvg:04fd7-g5");
    REQUIRE(get_child_id(8) == "kvg:04fd7-s9");
}

TEST_CASE("generate_stroke_order_svg_files") {
    std::string path = path_for_kanji("04fd7");
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

// for the found stroke nodes, delete them one by one, starting with the latest
// and write the svg. Then insert the DTD.

// cat 04fd7-4.svg | magick svg:- out.png
