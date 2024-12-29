
#include "stroke_order.hpp"

#include <cassert>
#include <cstring>
#include <regex>
#include <algorithm>
#include <fstream>
#include "util.hpp"

std::vector<std::vector<uint8_t>> code_point_to_stroke_order_jpgs(const std::string& code_point) {
    std::string path = path_for_kanji(code_point);
    std::vector<std::string> svgs = generate_stroke_order_svg_files(path);
    std::vector<std::vector<uint8_t>> jpgs {};

    std::transform(svgs.begin(),
                   svgs.end(),
                   std::back_inserter(jpgs),
                   svg_to_jpg
                   );

    return jpgs;
}

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

std::string read_file(std::string path) {
    std::ifstream ifstream { path.c_str() };
    std::stringstream buffer {};
    buffer << ifstream.rdbuf();
    return buffer.str();
}

std::vector<std::string> generate_stroke_order_svg_files(std::string path) {
    std::string xml_doc_as_string = read_file(path);

    std::regex regex { "<!DOCTYPE[^\\]]+]>" };
    std::smatch match {};
    std::regex_search(xml_doc_as_string, match, regex);
    assert(!match.empty());
    std::string dtd = match[0];

    pugi::xml_document doc {};
    assert(doc.load_string(xml_doc_as_string.c_str()));

    auto stroke_nodes = find_stroke_nodes(doc);

    std::vector<std::string> svg_files = {};
    for (int stroke_index = stroke_nodes.size(); stroke_index > 0; stroke_index--) {
        std::stringstream ofstream {};
        ofstream << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" << std::endl;
        ofstream << dtd << std::endl;

        doc.save(ofstream,
                 PUGIXML_TEXT("\t"),
                 pugi::format_no_declaration | pugi::format_indent
                 );
        std::string svg_file = ofstream.str();
        svg_files.push_back(svg_file);

        // Prepare for next iteration
        auto tuple = stroke_nodes[stroke_index-1];
        auto parent = std::get<0>(tuple);
        auto child = std::get<1>(tuple);
        parent.remove_child(child);
    }

    std::reverse(svg_files.begin(), svg_files.end());
    return svg_files;
}

std::vector<uint8_t> svg_to_jpg(std::string& svg) {
    std::string cmd = "echo '" + svg + "' | magick svg:- jpg:-";
    return exec(cmd);
}
