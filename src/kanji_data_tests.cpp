
#include "catch.hpp"
#include <string>
#include <iostream>

#include "kanji_data.hpp"

#ifdef TEST

TEST_CASE("test") {
    kanji_data kanji_data { "日" };
    kanji_data.read_from_file("../data/kanjidic2.xml");

    std::vector<std::string> meanings = kanji_data.get_meanings();
    REQUIRE(meanings.size() == 4);
    REQUIRE(meanings[0] == "day");
    REQUIRE(meanings[1] == "sun");
    REQUIRE(meanings[2] == "Japan");
    REQUIRE(meanings[3] == "counter for days");

    std::vector<std::string> kun_readings = kanji_data.get_kun_readings();
    REQUIRE(kun_readings.size() == 3);
    REQUIRE(kun_readings[0] == "ひ");
    REQUIRE(kun_readings[1] == "-び");
    REQUIRE(kun_readings[2] == "-か");

    std::vector<std::string> on_readings = kanji_data.get_on_readings();
    REQUIRE(on_readings.size() == 2);
    REQUIRE(on_readings[0] == "ニチ");
    REQUIRE(on_readings[1] == "ジツ");
}

#endif
