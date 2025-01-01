
/*
  This file contains a function that, given a unicode code point of a kanji,
  returns binary jpg data of diagrams showing the stroke order for that kanji.
 */

#ifndef HEADER_STROKE_ORDER_HPP
#define HEADER_STROKE_ORDER_HPP

#include <string>
#include <pugixml.hpp>
#include <vector>
#include <tuple>
#include <cstdint>

// Public API

const uint16_t stroke_order_image_size = 400;

std::vector<std::vector<uint8_t>> code_point_to_stroke_order_jpgs(const std::string& code_point);

// Private API
// (Exists for white-box testability)

std::string path_for_kanji(const std::string& code_point);

std::vector<std::tuple<pugi::xml_node, pugi::xml_node>>
    find_stroke_nodes(pugi::xml_document& doc);

void find_stroke_nodes(
                       pugi::xml_node current_node,
                       std::vector<std::tuple<pugi::xml_node, pugi::xml_node>>& parent_child_tuples
                       );

std::vector<std::string> generate_stroke_order_svg_files(std::string path);

std::vector<uint8_t> svg_to_jpg(std::string& svg);

#endif
