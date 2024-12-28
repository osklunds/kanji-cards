
#include "stroke_order.hpp"

#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <filesystem>
#include <pugixml.hpp>
#include <vector>
#include <regex>
#include <tuple>

TEST_CASE("path_for_kanji") {
    std::string path = path_for_kanji("04fd7");
    REQUIRE("../data/kanji/04fd7.svg" == path);
    REQUIRE(std::filesystem::exists(path));
}

void find_stroke_nodes(
                       pugi::xml_node current_node,
                       std::vector<std::tuple<pugi::xml_node, pugi::xml_node>>& stroke_nodes
                       ) {
    for (pugi::xml_node group_or_stroke : current_node.children()) {
        std::string id = group_or_stroke.attribute("id").value();

        std::regex regex("kvg:[0-9a-z]+-(s|g)[0-9]+");
        std::smatch match = {};
        std::regex_search(id, match, regex);
        assert(match.size() == 2);
        std::string type = match[1];

        if (type == "s") {
            auto tuple = std::make_tuple(current_node, group_or_stroke);
            stroke_nodes.push_back(tuple);
        } else {
            assert(type == "g");
            find_stroke_nodes(group_or_stroke, stroke_nodes);
        }
    }
}

TEST_CASE("temp") {
    pugi::xml_document doc {};
    pugi::xml_parse_result result = doc.load_file(path_for_kanji("04fd7").c_str());
    REQUIRE(result == true);

    pugi::xml_node svg = doc.child("svg");
    pugi::xml_node stroke_paths = svg.child("g");
    pugi::xml_node root = stroke_paths.child("g");

    std::vector<std::tuple<pugi::xml_node, pugi::xml_node>> stroke_nodes = {};

    find_stroke_nodes(root, stroke_nodes);

    for (auto tuple : stroke_nodes) {
        auto parent = std::get<0>(tuple);
        auto node = std::get<1>(tuple);

        std::cout << "oskar: " << node.attribute("id").value() << std::endl;

    }
}
