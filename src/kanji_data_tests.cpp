
#include "catch.hpp"
#include <string>
#include <string.h>
#include <iostream>
#include "pugixml.hpp"

#include "kanji_data.hpp"

TEST_CASE("sample_data") {
    pugi::xml_document kanjidic2_doc;
    pugi::xml_parse_result result_kanjidic2 =
        kanjidic2_doc.load_file("../data/kanjidic2_sample.xml");
    REQUIRE(result_kanjidic2 == true);

    pugi::xml_document jmdict_e_doc;
    pugi::xml_parse_result result_jmdict_e =
        jmdict_e_doc.load_file("../data/JMdict_e_sample.xml");
    REQUIRE(result_jmdict_e == true);
    
    kanji_data kanji_data { "日" };
    kanji_data.read_from_doc(kanjidic2_doc, jmdict_e_doc);

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

    std::vector<word_data> words = kanji_data.get_words();
    REQUIRE(words.size() == 2);

    REQUIRE(words[0].get_word() == "日本");
    REQUIRE(words[0].get_reading() == "にほん");
    REQUIRE(words[0].get_meanings() == (std::vector<std::string>){ "Japan" });
    REQUIRE(words[0].get_prio_news() == std::nullopt);
    REQUIRE(words[0].get_prio_ichi() == 1);
    REQUIRE(words[0].get_prio_spec() == std::nullopt);
    REQUIRE(words[0].get_prio_nf() == std::nullopt);

    REQUIRE(words[1].get_word() == "日");
    REQUIRE(words[1].get_reading() == "にち");
    REQUIRE(words[1].get_meanings() == (std::vector<std::string>){ "Day" });
    REQUIRE(words[1].get_prio_news() == std::nullopt);
    REQUIRE(words[1].get_prio_ichi() == std::nullopt);
    REQUIRE(words[1].get_prio_spec() == std::nullopt);
    REQUIRE(words[1].get_prio_nf() == std::nullopt);
}

TEST_CASE("as_string") {
    pugi::xml_document kanjidic2_doc;
    pugi::xml_parse_result result_kanjidic2 =
        kanjidic2_doc.load_file("../data/kanjidic2_sample.xml");
    REQUIRE(result_kanjidic2 == true);

    pugi::xml_document jmdict_e_doc;
    pugi::xml_parse_result result_jmdict_e =
        jmdict_e_doc.load_file("../data/JMdict_e_sample.xml");
    REQUIRE(result_jmdict_e == true);

    kanji_data kanji_data { "日" };
    kanji_data.read_from_doc(kanjidic2_doc, jmdict_e_doc);

    std::string string = kanji_data.as_string();
    std::string exp_string {};
    exp_string.append("-----\n");
    exp_string.append("Kanji: 日\n");
    exp_string.append("Meanings: day, sun, Japan, counter for days\n");
    exp_string.append("Kun readings: ひ, -び, -か\n");
    exp_string.append("On readings: ニチ, ジツ\n");
    exp_string.append("---\n");
    exp_string.append("Word: 日本\n");
    exp_string.append("Reading: にほん\n");
    exp_string.append("Meanings: Japan\n");
    exp_string.append("Prio news: \n");
    exp_string.append("Prio ichi: 1\n");
    exp_string.append("Prio spec: \n");
    exp_string.append("Prio nf: \n");
    exp_string.append("---\n");
    exp_string.append("Word: 日\n");
    exp_string.append("Reading: にち\n");
    exp_string.append("Meanings: Day\n");
    exp_string.append("Prio news: \n");
    exp_string.append("Prio ichi: \n");
    exp_string.append("Prio spec: \n");
    exp_string.append("Prio nf: \n");
    exp_string.append("-----");
    REQUIRE(exp_string == string);
}

