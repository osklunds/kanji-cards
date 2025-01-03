
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
  frequency_entry contains a kanji and it's frequency value based on
  kanji_frequency_list.csv. There's also a function that returns all
  frequency_entry objects based on that .csv file, sorted on frequency.
 */

#ifndef HEADER_FREQUENCY_HPP
#define HEADER_FREQUENCY_HPP

#include <vector>

class frequency_entry {
private:
    std::string kanji;
    int frequency;

public:
    frequency_entry(const std::string& kanji, int frequency);

    int get_frequency() const;
    const std::string& get_kanji() const;

    friend bool operator< (const frequency_entry& fe1, const frequency_entry& fe2);
};

std::vector<frequency_entry> get_frequency_entries_from_file(const std::string& file_path);

#endif

