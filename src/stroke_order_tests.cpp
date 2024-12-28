
#include "stroke_order.hpp"

#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <filesystem>
#include <pugixml.hpp>
#include <vector>
#include <regex>

TEST_CASE("path_for_kanji") {
    std::string path = path_for_kanji("04fd7");
    REQUIRE("../data/kanji/04fd7.svg" == path);
    REQUIRE(std::filesystem::exists(path));
}

void iterate(pugi::xml_node node, const pugi::xml_document& doc) {
    std::vector<pugi::xml_node> vec {node.children().begin(), node.children().end()};
    for (pugi::xml_node group_or_stroke : vec) {
        std::string id = group_or_stroke.attribute("id").value();

        std::regex regex("kvg:([0-9a-z]+-(s|g)[0-9]+)");
        std::smatch match = {};
        std::regex_search(id, match, regex);
        assert(match.size() == 3);
        std::string trimmed_id = match[1];
        std::string type = match[2];

        if (type == "s") {
            std::cout << "oskar: " << id << std::endl;
            std::string path = trimmed_id + ".svg";
            node.remove_child(group_or_stroke);
            doc.save_file(path.c_str());
        } else {
            assert(type == "g");
            iterate(group_or_stroke, doc);
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

    iterate(root, doc);
}
