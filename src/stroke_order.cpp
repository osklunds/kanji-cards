
#include "stroke_order.hpp"

#include <cassert>
#include <regex>

std::string path_for_kanji(const std::string& code_point) {
    assert(code_point.size() == 5);

    return "../data/kanji/" + code_point + ".svg";
}

std::vector<std::tuple<pugi::xml_node, pugi::xml_node>>
find_stroke_nodes(pugi::xml_document& doc) {
    pugi::xml_node svg = doc.child("svg");
    pugi::xml_node stroke_paths = svg.child("g");
    pugi::xml_node root = stroke_paths.child("g");

    std::vector<std::tuple<pugi::xml_node, pugi::xml_node>> stroke_nodes = {};

    find_stroke_nodes(root, stroke_nodes);

    return stroke_nodes;
}

void find_stroke_nodes(
                       pugi::xml_node parent,
                       std::vector<std::tuple<pugi::xml_node, pugi::xml_node>>& parent_child_tuples
                       ) {
    for (pugi::xml_node child : parent.children()) {
        std::string id = child.attribute("id").value();

        std::regex regex("kvg:[0-9a-z]+-(s|g)[0-9]+");
        std::smatch match = {};
        std::regex_search(id, match, regex);
        assert(match.size() == 2);
        std::string type = match[1];

        if (type == "s") {
            auto tuple = std::make_tuple(parent, child);
            parent_child_tuples.push_back(tuple);
        } else {
            assert(type == "g");
            find_stroke_nodes(child, parent_child_tuples);
        }
    }
}
