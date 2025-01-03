
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

#include <catch2/catch_test_macros.hpp>
#include <string>
#include <string.h>
#include <iostream>
#include "pugixml.hpp"

#include "kanji_data.hpp"

TEST_CASE("sample_data") {
    pugi::xml_document kanjidic2_doc;
    pugi::xml_parse_result result_kanjidic2 =
        kanjidic2_doc.load_file("kanjidic2.xml");
    REQUIRE(result_kanjidic2 == true);

    pugi::xml_document jmdict_e_doc;
    pugi::xml_parse_result result_jmdict_e =
        jmdict_e_doc.load_file("JMdict_e.xml.sample");
    REQUIRE(result_jmdict_e == true);
    
    kanji_data kanji_data { "日", 3, kanjidic2_doc, jmdict_e_doc };

    REQUIRE("日" == kanji_data.get_kanji());
    REQUIRE(3 == kanji_data.get_frequency());

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

    REQUIRE(words[0].get_word() == "日");
    REQUIRE(words[0].get_reading() == "ひ");

    std::vector<std::string> exp_meanings = {
        "day",
        "days",
        "sun",
        "sunshine",
        "sunlight",
        "(the) day",
        "daytime",
        "daylight",
        "date",
        "deadline",
        "(past) days",
        "time (e.g. of one's childhood)",
        "case (esp. unfortunate)",
        "event"
    };
    REQUIRE(words[0].get_meanings() == exp_meanings);
    REQUIRE(words[0].get_prio_news() == 1);
    REQUIRE(words[0].get_prio_ichi() == 1);
    REQUIRE(words[0].get_prio_spec() == std::nullopt);
    REQUIRE(words[0].get_prio_nf() == 3);

    REQUIRE(words[1].get_word() == "日本");
    REQUIRE(words[1].get_reading() == "にほん");
    REQUIRE(words[1].get_meanings() == (std::vector<std::string>){ "Japan" });
    REQUIRE(words[1].get_prio_news() == 2);
    REQUIRE(words[1].get_prio_ichi() == std::nullopt);
    REQUIRE(words[1].get_prio_spec() == 1);
    REQUIRE(words[1].get_prio_nf() == 25);

    REQUIRE(kanji_data.get_stroke_order_jpgs().size() == 4);
}

TEST_CASE("as_string") {
    pugi::xml_document kanjidic2_doc;
    pugi::xml_parse_result result_kanjidic2 =
        kanjidic2_doc.load_file("kanjidic2.xml");
    REQUIRE(result_kanjidic2 == true);

    pugi::xml_document jmdict_e_doc;
    pugi::xml_parse_result result_jmdict_e =
        jmdict_e_doc.load_file("JMdict_e.xml.sample");
    REQUIRE(result_jmdict_e == true);

    kanji_data kanji_data { "日", 1, kanjidic2_doc, jmdict_e_doc };

    std::string string = kanji_data.as_string();
    std::string exp_string {};
    exp_string.append("-----\n");
    exp_string.append("Kanji: 日\n");
    exp_string.append("Meanings: day, sun, Japan, counter for days\n");
    exp_string.append("Kun readings: ひ, -び, -か\n");
    exp_string.append("On readings: ニチ, ジツ\n");
    exp_string.append("---\n");
    exp_string.append("Word: 日\n");
    exp_string.append("Reading: ひ\n");
    exp_string.append("Meanings: day, days, sun, sunshine, sunlight, (the) day, daytime, daylight, date, deadline, (past) days, time (e.g. of one's childhood), case (esp. unfortunate), event\n");
    exp_string.append("Prio news: 1\n");
    exp_string.append("Prio ichi: 1\n");
    exp_string.append("Prio spec: \n");
    exp_string.append("Prio nf: 3\n");
    exp_string.append("---\n");
    exp_string.append("Word: 日本\n");
    exp_string.append("Reading: にほん\n");
    exp_string.append("Meanings: Japan\n");
    exp_string.append("Prio news: 2\n");
    exp_string.append("Prio ichi: \n");
    exp_string.append("Prio spec: 1\n");
    exp_string.append("Prio nf: 25\n");
    exp_string.append("-----");
    REQUIRE(exp_string == string);
}

