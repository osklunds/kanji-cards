
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

#include "util.hpp"

#include <cassert>
#include <iostream>
#include <memory>
#include <fstream>
#include <regex>

// Inspired by https://gist.github.com/meritozh/f0351894a2a4aa92871746bf45879157
std::vector<uint8_t> exec(std::string cmd) {
    std::shared_ptr<FILE> pipe { popen(cmd.c_str(), "r"), pclose };
    assert(pipe);

    std::vector<uint8_t> result {};

    while (!feof(pipe.get())) {
        result.push_back(fgetc(pipe.get()));
    }

    // For some reason, 'm' is added in the end
    result.pop_back();

    return result;
}

std::vector<uint8_t> read_file_bin(std::string path) {
    std::ifstream input { path.c_str(), std::ios::binary };
    std::vector<uint8_t> buffer(std::istreambuf_iterator<char>(input), {});
    return buffer;
}

bool regex_matches(std::string subject, std::string regex_string) {
    std::regex regex(regex_string);
    std::smatch match {};
    regex_search(subject, match, regex);
    return !match.empty();
}
