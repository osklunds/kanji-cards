
#include <iostream>

#include "catch.hpp"

#include "frequency.hpp"

#ifdef TEST

TEST_CASE("get_frequency_entries_from_file") {
    std::vector<frequency_entry> entries =
        get_frequency_entries_from_file("../data/kanji_freqency_list_sample.csv");

    REQUIRE(entries[0].get_kanji() == "一");
    REQUIRE(entries[0].get_frequency() == 10);

    REQUIRE(entries[1].get_kanji() == "二");
    REQUIRE(entries[1].get_frequency() == 97);

    REQUIRE(entries[2].get_kanji() == "四");
    REQUIRE(entries[2].get_frequency() == 97);

    REQUIRE(entries[3].get_kanji() == "仁");
    REQUIRE(entries[3].get_frequency() == 432);

    REQUIRE(entries.size() == 4);
}

#endif
