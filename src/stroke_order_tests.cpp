
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

TEST_CASE("temp") {
    pugi::xml_document doc {};
    pugi::xml_parse_result result = doc.load_file(path_for_kanji("04fd7").c_str());
    REQUIRE(result == true);

    std::ifstream ifstream(path_for_kanji("04fd7").c_str());
    std::stringstream buffer;
    buffer << ifstream.rdbuf();
    std::string xml_doc_as_string = buffer.str();

    std::regex regex("<!DOCTYPE[^\\]]+]>");
    std::smatch match = {};
    std::regex_search(xml_doc_as_string, match, regex);
    REQUIRE(!match.empty());

    std::string dtd = match[0];

    auto stroke_nodes = find_stroke_nodes(doc);

    for (int stroke_index = stroke_nodes.size(); stroke_index > 0; stroke_index--) {
        std::string path = "04fd7-" + std::to_string(stroke_index) + ".svg";

        std::ofstream ofstream(path);
        ofstream << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" << std::endl;
        ofstream << dtd << std::endl;

        doc.save(ofstream,
                 PUGIXML_TEXT("\t"),
                 pugi::format_no_declaration | pugi::format_indent
                 );
        ofstream.close();

        // Prepare for next iteration
        auto tuple = stroke_nodes[stroke_index-1];
        auto parent = std::get<0>(tuple);
        auto child = std::get<1>(tuple);
        parent.remove_child(child);
    }
}

// for the found stroke nodes, delete them one by one, starting with the latest
// and write the svg. Then insert the DTD.
