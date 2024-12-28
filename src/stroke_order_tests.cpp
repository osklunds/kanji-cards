
#include "stroke_order.hpp"

#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <filesystem>

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

    REQUIRE(get_parent_id(0) == "kvg:04fd7-g5");
    REQUIRE(get_child_id(0) == "kvg:04fd7-s9");

    REQUIRE(get_parent_id(1) == "kvg:04fd7-g5");
    REQUIRE(get_child_id(1) == "kvg:04fd7-s8");

    REQUIRE(get_parent_id(2) == "kvg:04fd7-g5");
    REQUIRE(get_child_id(2) == "kvg:04fd7-s7");

    REQUIRE(get_parent_id(3) == "kvg:04fd7-g4");
    REQUIRE(get_child_id(3) == "kvg:04fd7-s6");

    REQUIRE(get_parent_id(4) == "kvg:04fd7-g4");
    REQUIRE(get_child_id(4) == "kvg:04fd7-s5");

    REQUIRE(get_parent_id(5) == "kvg:04fd7-g3");
    REQUIRE(get_child_id(5) == "kvg:04fd7-s4");

    REQUIRE(get_parent_id(6) == "kvg:04fd7-g3");
    REQUIRE(get_child_id(6) == "kvg:04fd7-s3");

    REQUIRE(get_parent_id(7) == "kvg:04fd7-g1");
    REQUIRE(get_child_id(7) == "kvg:04fd7-s2");

    REQUIRE(get_parent_id(8) == "kvg:04fd7-g1");
    REQUIRE(get_child_id(8) == "kvg:04fd7-s1");
}

// for the found stroke nodes, delete them one by one, starting with the latest
// and write the svg. Then insert the DTD.
