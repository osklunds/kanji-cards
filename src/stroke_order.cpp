
#include "stroke_order.hpp"

#include <cassert>
#include <cstring>
#include <regex>
#include <algorithm>
#include <fstream>
#include "util.hpp"
#include <iostream>
#include <format>

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
    assert(code_point.size() == 4 || code_point.size() == 5);
    std::string padding {};
    if (code_point.size() == 4) {
        padding = "0";
    }

    return "../data/kanji/" + padding + code_point + ".svg";
}

std::vector<std::tuple<pugi::xml_node, pugi::xml_node>>
find_stroke_nodes(pugi::xml_document& doc) {
    pugi::xml_node svg = doc.child("svg");
    pugi::xpath_node xpath_stroke_paths =
        svg.select_node("g[@id[contains(.,\"StrokePaths\")]]");
    pugi::xml_node stroke_paths = xpath_stroke_paths.node();
    assert(!stroke_paths.empty());
    pugi::xml_node root = stroke_paths.child("g");

    std::vector<std::tuple<pugi::xml_node, pugi::xml_node>> stroke_nodes = {};

    find_stroke_nodes(root, stroke_nodes);

    auto node_to_integer_index = [](pugi::xml_node node) {
        std::string id = node.attribute("id").value();
        std::regex regex("kvg:[0-9a-z]+-s([0-9]+)");
        std::smatch match = {};
        std::regex_search(id, match, regex);
        assert(match.size() == 2);
        std::string index = match[1];
        // Need stoi so that 1 < 2 < 10
        return std::stoi(index);
    };

    std::ranges::sort(stroke_nodes, [node_to_integer_index](auto a, auto b) {
        auto nodeA = std::get<1>(a);
        auto nodeB = std::get<1>(b);

        return node_to_integer_index(nodeA) < node_to_integer_index(nodeB);
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

std::vector<std::string> generate_stroke_order_svg_files(std::string file_path) {
    std::string xml_doc_as_string = read_file(file_path);

    std::regex regex { "<!DOCTYPE[^\\]]+]>" };
    std::smatch match {};
    std::regex_search(xml_doc_as_string, match, regex);
    assert(!match.empty());
    std::string dtd = match[0];

    pugi::xml_document doc {};
    assert(doc.load_string(xml_doc_as_string.c_str()));

    pugi::xml_node svg = doc.child("svg");
    assert((std::string)svg.attribute("width").value() == "109");
    assert((std::string)svg.attribute("height").value() == "109");
    assert((std::string)svg.attribute("viewBox").value() == "0 0 109 109");

    pugi::xpath_node xpath_stroke_numbers =
        svg.select_node("g[@id[contains(.,\"StrokeNumbers\")]]");
    pugi::xml_node stroke_numbers = xpath_stroke_numbers.node();
    assert(!stroke_numbers.empty());
    svg.remove_child(stroke_numbers);

    auto stroke_nodes = find_stroke_nodes(doc);

    for (auto tuple : stroke_nodes) {
        auto child = std::get<1>(tuple);
        child.append_attribute("stroke") = "rgb(180, 180, 180)";
    }

    std::vector<std::string> svg_files = {};
    for (int stroke_index = stroke_nodes.size(); stroke_index > 0; stroke_index--) {
        std::stringstream ofstream {};
        ofstream << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" << std::endl;
        ofstream << dtd << std::endl;

        auto tuple = stroke_nodes[stroke_index-1];
        auto parent = std::get<0>(tuple);
        auto child = std::get<1>(tuple);

        // Example
        // path attribute of M30.5,17.89c0.12,1.12-0.03...
        // Should result in the below extra node
        // <circle cx="30.5" cy="17.89" r="4" fill="red" stroke-width="0"></circle>
        std::string path = child.attribute("d").value();
        // todo: test for c and C. Failed for 04e2d
        // check if point is located corretly
        std::regex regex("M([0-9]+\\.?[0-9]*),([0-9]+\\.?[0-9]*)(c|C)");
        std::smatch match {};
        std::regex_search(path, match, regex);
        assert(match.size() == 4);
        std::string x_pos = match[1];
        std::string y_pos = match[2];

        pugi::xml_node circle = parent.append_child("circle");
        circle.append_attribute("cx") = x_pos;
        circle.append_attribute("cy") = y_pos;
        circle.append_attribute("r") = "4";
        circle.append_attribute("fill") = "red";
        circle.append_attribute("stroke-width") = "0";

        child.remove_attribute("stroke");

        doc.save(ofstream,
                 PUGIXML_TEXT("\t"),
                 pugi::format_no_declaration | pugi::format_indent
                 );
        std::string svg_file = ofstream.str();
        svg_files.push_back(svg_file);

        parent.remove_child(circle);
        parent.remove_child(child);
    }

    std::reverse(svg_files.begin(), svg_files.end());
    return svg_files;
}

std::vector<uint8_t> svg_to_jpg(std::string& svg) {
    std::string cmd = std::format("echo '{}' | magick -size {}x{} svg:- jpg:-",
                                  svg,
                                  stroke_order_image_size,
                                  stroke_order_image_size
                                  );
    return exec(cmd);
}
