
#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <tuple>
#include <functional>

#include "word_data.hpp"

TEST_CASE("word_data_sample_data_read_from_doc_1_match") {
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file("JMdict_e.xml.sample");
    REQUIRE(result == true);

    std::vector<word_data> word_datas = word_data::read_from_doc(doc, "語");
    REQUIRE(word_datas.size() == 1);

    word_data word_data = word_datas[0];
    REQUIRE(word_data.get_word() == "物語");

    std::vector<std::string> meanings = {
        "story",
        "tale",
        "narrative",
        "account",
        "fable",
        "legend"
    };
    REQUIRE(word_data.get_meanings() == meanings);

    REQUIRE("ものがたり" == word_data.get_reading());
    REQUIRE(1 == word_data.get_prio_news());
    REQUIRE(1 == word_data.get_prio_ichi());
    REQUIRE(std::nullopt == word_data.get_prio_spec());
    REQUIRE(3 == word_data.get_prio_nf());
}

TEST_CASE("word_data_sample_data_read_from_doc_2_matches") {
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file("JMdict_e.xml.sample");
    REQUIRE(result == true);

    std::vector<word_data> word_datas = word_data::read_from_doc(doc, "動");
    REQUIRE(word_datas.size() == 2);

    word_data animal = word_datas[0];
    REQUIRE(animal.get_word() == "動物");

    std::vector<std::string> animal_meanings = { "animal" };
    REQUIRE(animal.get_meanings() == animal_meanings);

    REQUIRE("どうぶつ" == animal.get_reading());
    REQUIRE(1 == animal.get_prio_news());
    REQUIRE(1 == animal.get_prio_ichi());
    REQUIRE(std::nullopt == animal.get_prio_spec());
    REQUIRE(2 == animal.get_prio_nf());

    word_data to_be_perturbed = word_datas[1];
    REQUIRE(to_be_perturbed.get_word() == "動じる");

    std::vector<std::string> to_be_perturbed_meanings = {
        "to be perturbed",
        "to be agitated"
    };
    REQUIRE(to_be_perturbed.get_meanings() == to_be_perturbed_meanings);

    REQUIRE("どうじる" == to_be_perturbed.get_reading());
    REQUIRE(2 == to_be_perturbed.get_prio_news());
    REQUIRE(1 == to_be_perturbed.get_prio_ichi());
    REQUIRE(std::nullopt == to_be_perturbed.get_prio_spec());
    REQUIRE(33 == to_be_perturbed.get_prio_nf());

}

TEST_CASE("word_data_as_string") {
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file("JMdict_e.xml.sample");
    REQUIRE(result == true);

    std::vector<word_data> word_datas = word_data::read_from_doc(doc, "語");
    std::string string = word_datas[0].as_string();

    std::string exp_string {};
    exp_string.append("Word: 物語\n");
    exp_string.append("Reading: ものがたり\n");
    exp_string.append("Meanings: story, tale, narrative, account, fable, legend\n");
    exp_string.append("Prio news: 1\n");
    exp_string.append("Prio ichi: 1\n");
    exp_string.append("Prio spec: \n");
    exp_string.append("Prio nf: 3");

    REQUIRE(exp_string == string);
}

TEST_CASE("word_data_setters") {
    word_data word = {};
    word.set_word("日");
    word.set_prio_news(3);
    word.set_prio_ichi(2);
    word.set_prio_spec(1);
    word.set_prio_nf(12);

    REQUIRE("日" == word.get_word());
    REQUIRE(3 == word.get_prio_news());
    REQUIRE(2 == word.get_prio_ichi());
    REQUIRE(1 == word.get_prio_spec());
    REQUIRE(12 == word.get_prio_nf());
}

TEST_CASE("compare_word_data_with_one_prio_type") {
    word_data none  = {};

    word_data nf1 = {};
    nf1.set_prio_nf(1);

    word_data nf2 = {};
    nf2.set_prio_nf(2);

    REQUIRE(nf1 < none);
    REQUIRE(nf1 < nf2);


    word_data nf3 = {};
    nf3.set_prio_nf(3);

    word_data nf4 = {};
    nf4.set_prio_nf(4);

    word_data spec1 = {};
    spec1.set_prio_spec(1);

    REQUIRE(nf2 < spec1);
    REQUIRE(nf3 == spec1);
    REQUIRE(nf4 > spec1);
    REQUIRE(spec1 < none);


    word_data nf5 = {};
    nf5.set_prio_nf(5);

    word_data nf6 = {};
    nf6.set_prio_nf(6);

    word_data nf7 = {};
    nf7.set_prio_nf(7);

    word_data spec2 = {};
    spec2.set_prio_spec(2);

    REQUIRE(nf5 < spec2);
    REQUIRE(nf6 == spec2);
    REQUIRE(nf7 > spec2);
    REQUIRE(spec1 < spec2);
    REQUIRE(spec2 < none);


    word_data nf17 = {};
    nf17.set_prio_nf(17);

    word_data nf18 = {};
    nf18.set_prio_nf(18);

    word_data nf19 = {};
    nf19.set_prio_nf(19);

    word_data ichi1 = {};
    ichi1.set_prio_ichi(1);

    word_data ichi2 = {};
    ichi2.set_prio_ichi(2);

    REQUIRE(nf17 < ichi1);
    REQUIRE(nf18 == ichi1);
    REQUIRE(nf19 > ichi1);
    REQUIRE(ichi1 == ichi2);
    REQUIRE(ichi1 < none);
    REQUIRE(ichi2 < none);


    word_data nf22 = {};
    nf22.set_prio_nf(22);

    word_data nf23 = {};
    nf23.set_prio_nf(23);

    word_data nf24 = {};
    nf24.set_prio_nf(24);

    word_data news1 = {};
    news1.set_prio_news(1);

    REQUIRE(nf22 < news1);
    REQUIRE(nf23 == news1);
    REQUIRE(nf24 > news1);
    REQUIRE(news1 < none);


    word_data nf43 = {};
    nf43.set_prio_nf(43);

    word_data nf44 = {};
    nf44.set_prio_nf(44);

    word_data nf45 = {};
    nf45.set_prio_nf(45);

    word_data news2 = {};
    news2.set_prio_news(2);

    REQUIRE(nf43 < news2);
    REQUIRE(nf44 == news2);
    REQUIRE(nf45 > news2);
    REQUIRE(news1 < news2);
    REQUIRE(news2 < none);
}
}
