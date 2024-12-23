
#include "catch.hpp"
#include <iostream>

#include "word_data.hpp"

#ifdef TEST

TEST_CASE("word_data_sample_data_read_from_doc_1_match") {
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file("../data/JMdict_e_sample.xml");
    REQUIRE(result == true);

    std::vector<word_data> word_datas = word_data::read_from_doc(doc, "語");
    REQUIRE(word_datas.size() == 1);

    word_data word_data = word_datas[0];
    REQUIRE(word_data.get_word() == "物語");

    std::vector<std::string> meanings = { "story", "tale", "legend" };
    REQUIRE(word_data.get_meanings() == meanings);

    REQUIRE("ものがたり" == word_data.get_reading());
    REQUIRE("news2" == word_data.get_prio_news());
    REQUIRE("ichi1" == word_data.get_prio_ichi());
    REQUIRE("spec2" == word_data.get_prio_spec());
    REQUIRE("nf41" == word_data.get_prio_nf());
}

TEST_CASE("word_data_sample_data_read_from_doc_2_matches") {
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file("../data/JMdict_e_sample.xml");
    REQUIRE(result == true);

    std::vector<word_data> word_datas = word_data::read_from_doc(doc, "動");
    REQUIRE(word_datas.size() == 2);

    word_data animal = word_datas[0];
    REQUIRE(animal.get_word() == "動物");

    std::vector<std::string> animal_meanings = { "animal" };
    REQUIRE(animal.get_meanings() == animal_meanings);

    REQUIRE("どうぶつ" == animal.get_reading());
    REQUIRE("" == animal.get_prio_news());
    REQUIRE("" == animal.get_prio_ichi());
    REQUIRE("" == animal.get_prio_spec());
    REQUIRE("" == animal.get_prio_nf());

    word_data to_move = word_datas[1];
    REQUIRE(to_move.get_word() == "動く");

    std::vector<std::string> to_move_meanings = { "to move" };
    REQUIRE(to_move.get_meanings() == to_move_meanings);

    REQUIRE("うごく" == to_move.get_reading());
    REQUIRE("" == to_move.get_prio_news());
    REQUIRE("" == to_move.get_prio_ichi());
    REQUIRE("" == to_move.get_prio_spec());
    REQUIRE("" == to_move.get_prio_nf());
}

TEST_CASE("word_data_as_string") {
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file("../data/JMdict_e_sample.xml");
    REQUIRE(result == true);

    std::vector<word_data> word_datas = word_data::read_from_doc(doc, "語");
    std::string string = word_datas[0].as_string();

    std::string exp_string {};
    exp_string.append("Word: 物語\n");
    exp_string.append("Reading: ものがたり\n");
    exp_string.append("Meanings: story, tale, legend\n");
    exp_string.append("Prio news: news2\n");
    exp_string.append("Prio ichi: ichi1\n");
    exp_string.append("Prio spec: spec2\n");
    exp_string.append("Prio nf: nf41");

    REQUIRE(exp_string == string);
}

#endif
