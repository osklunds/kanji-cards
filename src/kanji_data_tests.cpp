
#include "catch.hpp"
#include <string>
#include <iostream>

#include "kanji_data.hpp"

#ifdef TEST

TEST_CASE("test") {
    kanji_data kanji_data { "日" };
    kanji_data.read_from_file("../data/kanjidic2.xml");

    std::vector<std::string> meanings = kanji_data.get_meanings();
    REQUIRE(meanings[0] == "day");
    REQUIRE(meanings[1] == "sun");
    REQUIRE(meanings[2] == "Japan");
    REQUIRE(meanings[3] == "counter for days");
    REQUIRE(meanings.size() == 4);
}

#endif
