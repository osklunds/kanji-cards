
#ifndef HEADER_STROKE_ORDER_HPP
#define HEADER_STROKE_ORDER_HPP

#include <string>
#include <pugixml.hpp>
#include <vector>
#include <tuple>

// Public API

// Private API
// (Exists for white-box testability)

std::string path_for_kanji(const std::string& code_point);

std::vector<std::tuple<pugi::xml_node, pugi::xml_node>>
    find_stroke_nodes(pugi::xml_document& doc);
void find_stroke_nodes(
                       pugi::xml_node current_node,
                       std::vector<std::tuple<pugi::xml_node, pugi::xml_node>>& parent_child_tuples
                       );

#endif