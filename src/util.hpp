
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
  Utility functions not really domain-specific or related to a single other
  file.
 */

#ifndef HEADER_UTIL_HPP
#define HEADER_UTIL_HPP

#include <vector>
#include <string>
#include <cstdint>

std::vector<uint8_t> exec(std::string cmd);

std::vector<uint8_t> read_file_bin(std::string path);

bool regex_matches(std::string subject, std::string regex);

#endif
