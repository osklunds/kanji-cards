
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

#include <catch2/catch_test_macros.hpp>

#include "frequency.hpp"

TEST_CASE("get_frequency_entries_from_file") {
    std::vector<frequency_entry> entries =
        get_frequency_entries_from_file("kanji_frequency_list.csv");

    REQUIRE(entries.size() == 2242);

    REQUIRE(entries[0].get_kanji() == "人");
    REQUIRE(entries[0].get_frequency() == 8);

    REQUIRE(entries[1].get_kanji() == "日");
    REQUIRE(entries[1].get_frequency() == 10);

    REQUIRE(entries[43].get_kanji() == "理");
    REQUIRE(entries[43].get_frequency() == 88);
    
    REQUIRE(entries[2241].get_kanji() == "𠮟");
    REQUIRE(entries[2241].get_frequency() == 5308);
}
