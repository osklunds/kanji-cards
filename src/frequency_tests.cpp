
#include <iostream>

#include "catch.hpp"

#include "frequency.hpp"

#ifdef TEST

TEST_CASE("get_frequency_entries_from_file") {
    std::vector<frequency_entry> entries =
        get_frequency_entries_from_file("../data/kanji_freqency_list.csv");

    REQUIRE(entries[0].get_frequency() == 11);
    REQUIRE(entries[0].get_kanji() == "一");
    REQUIRE(entries[1].get_frequency() == 128);
    REQUIRE(entries[1].get_kanji() == "二");
    REQUIRE(entries[2].get_frequency() == 1254);
    REQUIRE(entries[2].get_kanji() == "仁");
}

#endif
