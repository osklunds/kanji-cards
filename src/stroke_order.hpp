
// Copyright (C) 2025 Oskar Lundström

// This file is part of kanji-cards.

// kanji-cards is free software: you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free Software
// Foundation, either version 3 of the License, or (at your option) any later
// version.

// kanji-cards is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
// FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
// details.

// You should have received a copy of the GNU General Public License along with
// kanji-cards. If not, see <https://www.gnu.org/licenses/>.

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
