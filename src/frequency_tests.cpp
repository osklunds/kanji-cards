
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
