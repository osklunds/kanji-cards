
/*
  Using kanji_data, create_card() creates a PDF file with informatin about
  the kanji.
 */

#ifndef HEADER_CREATE_CARD_HPP
#define HEADER_CREATE_CARD_HPP

#include <string>
#include "kanji_data.hpp"

std::string create_card(const kanji_data& kanji_data, const std::string& dir_path);

#endif
