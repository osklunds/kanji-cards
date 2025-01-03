
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

#include <iostream>
#include <regex>
#include <string>
#include <cassert>
#include <fstream>

#include "frequency.hpp"

frequency_entry::frequency_entry(const std::string& kanji, int frequency) :
    kanji { kanji },
    frequency { frequency }
{

}

int frequency_entry::get_frequency() const {
    return frequency;
}

const std::string& frequency_entry::get_kanji() const {
    return kanji;
}

std::vector<frequency_entry> get_frequency_entries_from_file(const std::string& file_path) {
    std::ifstream file(file_path);
    assert(file.is_open());

    std::vector<frequency_entry> entries;

    std::string line;
    while (std::getline(file, line)) {
        std::regex regex("(.*),(\\d+,){9}(\\d+)");
        std::smatch match;
        std::regex_search(line, match, regex);

        if (!match.empty()) {
            assert(match.size() == 4);
            std::string kanji = match[1];
            int frequency = std::stoi(match[3]);
            frequency_entry entry(kanji, frequency);
            entries.push_back(entry);
        }
    }
    file.close();

    std::sort(entries.begin(), entries.end());

    return entries;
}

bool operator< (const frequency_entry& fe1, const frequency_entry& fe2) {
    if (fe1.frequency == fe2.frequency) {
        return fe1.kanji < fe2.kanji;
    } else {
        return fe1.frequency < fe2.frequency;
    }
}

