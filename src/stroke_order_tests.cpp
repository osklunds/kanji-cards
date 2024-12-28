
#include "stroke_order.hpp"

#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <filesystem>

TEST_CASE("path_for_kanji") {
    std::string path = path_for_kanji("04fd7");
    REQUIRE("../data/kanji/04fd7.svg" == path);
    REQUIRE(std::filesystem::exists(path));
}

TEST_CASE("temp") {
    pugi::xml_document doc {};
    pugi::xml_parse_result result = doc.load_file(path_for_kanji("04fd7").c_str());
    REQUIRE(result == true);

    auto stroke_nodes = find_stroke_nodes(doc);

    for (auto tuple : stroke_nodes) {
        auto parent = std::get<0>(tuple);
        auto node = std::get<1>(tuple);

        std::cout << "oskar: " << node.attribute("id").value() << std::endl;

    }
}
