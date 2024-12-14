
#include "catch.hpp"
#include <string>
#include <string.h>
#include <iostream>
#include "pugixml.hpp"

#include "kanji_data.hpp"

#ifdef TEST

TEST_CASE("sample_data") {
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file("../data/kanjidic2.xml");
    kanji_data kanji_data { "日" };
    kanji_data.read_from_doc(doc);

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

TEST_CASE("as_string") {
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file("../data/kanjidic2.xml");
    kanji_data kanji_data { "日" };
    kanji_data.read_from_doc(doc);

    std::string string = kanji_data.as_string();
    std::string exp_string {};
    exp_string.append("Kanji: 日\n");
    exp_string.append("Meanings: day, sun, Japan, counter for days\n");
    exp_string.append("Kun readings: ひ, -び, -か\n");
    exp_string.append("On readings: ニチ, ジツ");

    REQUIRE(exp_string == string);
}

#endif
