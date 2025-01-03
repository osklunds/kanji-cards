
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

#include <catch2/catch_test_macros.hpp>

TEST_CASE("exec") {
    // Compare exec with known output
    REQUIRE(exec("echo hej") == (std::vector<uint8_t>){'h', 'e', 'j', '\n'});

    // Compare exec and read_file_bin for text file
    REQUIRE(exec("cat JMdict_e.xml.sample") == read_file_bin("JMdict_e.xml.sample"));

    // Compare exec and read_file_bin for binary file
    auto binResult = exec("cat main");
    REQUIRE(binResult == read_file_bin("main"));
    // Safety check to see that it's indeed a big file
    REQUIRE(binResult.size() > 1000*1000);
}
