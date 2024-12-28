
#include "stroke_order.hpp"

#include <cassert>
#include <cstring>
#include <regex>
#include <algorithm>
#include <fstream>

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

    std::ranges::sort(stroke_nodes,
                      [](auto a, auto b) {
                          auto nodeA = std::get<1>(a);
                          std::string nodeAId = nodeA.attribute("id").value();
                          auto nodeB = std::get<1>(b);
                          std::string nodeBId = nodeB.attribute("id").value();

                          return nodeAId < nodeBId;
                      });

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

std::vector<std::string> generate_stroke_order_svg_files(std::string path) {
    pugi::xml_document doc {};
    pugi::xml_parse_result result = doc.load_file(path.c_str());
    assert(result);

    std::ifstream ifstream(path.c_str());
    std::stringstream buffer;
    buffer << ifstream.rdbuf();
    std::string xml_doc_as_string = buffer.str();

    std::regex regex("<!DOCTYPE[^\\]]+]>");
    std::smatch match = {};
    std::regex_search(xml_doc_as_string, match, regex);
    assert(!match.empty());

    std::string dtd = match[0];

    auto stroke_nodes = find_stroke_nodes(doc);

    std::vector<std::string> svg_files = {};
    for (int stroke_index = stroke_nodes.size(); stroke_index > 0; stroke_index--) {
        std::string path = "04fd7-" + std::to_string(stroke_index) + ".svg";

        std::stringstream ofstream = {};
        ofstream << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" << std::endl;
        ofstream << dtd << std::endl;

        doc.save(ofstream,
                 PUGIXML_TEXT("\t"),
                 pugi::format_no_declaration | pugi::format_indent
                 );
        std::string out = ofstream.str();
        svg_files.push_back(out);

        // Prepare for next iteration
        auto tuple = stroke_nodes[stroke_index-1];
        auto parent = std::get<0>(tuple);
        auto child = std::get<1>(tuple);
        parent.remove_child(child);
    }

    std::reverse(svg_files.begin(), svg_files.end());
    return svg_files;
}
