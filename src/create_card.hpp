
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
  Using kanji_data, create_card() creates a PDF file with informatin about
  the kanji.
 */

#ifndef HEADER_CREATE_CARD_HPP
#define HEADER_CREATE_CARD_HPP

#include <string>
#include "kanji_data.hpp"

std::string create_card(const kanji_data& kanji_data, const std::string& dir_path);

#endif
