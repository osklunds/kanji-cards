
#include "catch.hpp"
#include <iostream>
#include <tuple>
#include <functional>

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
    REQUIRE(2 == word_data.get_prio_news());
    REQUIRE(1 == word_data.get_prio_ichi());
    REQUIRE(2 == word_data.get_prio_spec());
    REQUIRE(41 == word_data.get_prio_nf());
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
    REQUIRE(std::nullopt == animal.get_prio_news());
    REQUIRE(std::nullopt == animal.get_prio_ichi());
    REQUIRE(std::nullopt == animal.get_prio_spec());
    REQUIRE(std::nullopt == animal.get_prio_nf());

    word_data to_move = word_datas[1];
    REQUIRE(to_move.get_word() == "動く");

    std::vector<std::string> to_move_meanings = { "to move" };
    REQUIRE(to_move.get_meanings() == to_move_meanings);

    REQUIRE("うごく" == to_move.get_reading());
    REQUIRE(std::nullopt == to_move.get_prio_news());
    REQUIRE(std::nullopt == to_move.get_prio_ichi());
    REQUIRE(std::nullopt == to_move.get_prio_spec());
    REQUIRE(std::nullopt == to_move.get_prio_nf());
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
    exp_string.append("Prio news: 2\n");
    exp_string.append("Prio ichi: 1\n");
    exp_string.append("Prio spec: 2\n");
    exp_string.append("Prio nf: 41");

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

// This test is about comparing one prio metric to the same prio metric
// in the other word_data.
TEST_CASE("word_data_compare_one_prio") {
    std::vector<std::tuple<std::optional<int>, std::optional<int>, std::weak_ordering>>
        tests = 
        {
            std::make_tuple(std::nullopt, std::nullopt, std::weak_ordering::equivalent),
            std::make_tuple(std::nullopt,            1, std::weak_ordering::greater),
            std::make_tuple(std::nullopt,            2, std::weak_ordering::greater),

            std::make_tuple(1,            std::nullopt, std::weak_ordering::less),
            std::make_tuple(1,                       1, std::weak_ordering::equivalent),
            std::make_tuple(1,                       2, std::weak_ordering::less),

            std::make_tuple(2,            std::nullopt, std::weak_ordering::less),
            std::make_tuple(2,                       1, std::weak_ordering::greater),
            std::make_tuple(2,                       2, std::weak_ordering::equivalent)
        };

    std::vector<std::function<void(word_data&, std::optional<int>)>> set_prio_functions =
        {
            [](word_data& w, std::optional<int> prio) { w.set_prio_nf(prio); },
            [](word_data& w, std::optional<int> prio) { w.set_prio_spec(prio); },
            [](word_data& w, std::optional<int> prio) { w.set_prio_ichi(prio); },
            [](word_data& w, std::optional<int> prio) { w.set_prio_news(prio); }
        };

    for (auto set_prio : set_prio_functions) {
        for (auto test : tests) {
            word_data w1 = {};
            word_data w2 = {};

            set_prio(w1, std::get<0>(test));
            set_prio(w2, std::get<1>(test));

            auto result = w1 <=> w2;

            if (result != std::get<2>(test)) {
                std::cout << w1 << std::endl << std::endl;
                std::cout << w2 << std::endl << std::endl;
            }

            REQUIRE(result == std::get<2>(test));
        }
    }
}

// These tests are about comparing the order in which prio metrics are checked
TEST_CASE("word_data_compare_nf_before_spec1") {
    word_data w1 = {};
    word_data w2 = {};

    w1.set_prio_spec(1);
    w2.set_prio_spec(std::nullopt);
    REQUIRE(w1 < w2);

    w1.set_prio_nf(20);
    w2.set_prio_nf(19);
    REQUIRE(w1 > w2);
}

TEST_CASE("word_data_compare_spec1_ichi1_spec2") {
    word_data w1 = {};
    word_data w2 = {};

    // ichi1 decides
    w1.set_prio_ichi(1);
    w2.set_prio_ichi(std::nullopt);
    REQUIRE(w1 < w2);

    // spec2 doesn't affect ichi1
    w1.set_prio_spec(std::nullopt);
    w2.set_prio_spec(2);
    REQUIRE(w1 < w2);

    // spec1 affects ichi1
    w1.set_prio_spec(std::nullopt);
    w2.set_prio_spec(1);
    REQUIRE(w1 > w2);
}

TEST_CASE("word_data_compare_prio_order") {
    word_data spec1 = {};
    spec1.set_prio_spec(1);

    word_data ichi1 = {};
    ichi1.set_prio_ichi(1);

    word_data news1 = {};
    news1.set_prio_news(1);

    word_data spec2 = {};
    spec2.set_prio_spec(2);

    word_data ichi2 = {};
    ichi2.set_prio_ichi(2);

    word_data news2 = {};
    news2.set_prio_news(2);

    word_data nf1 = {};
    nf1.set_prio_nf(1);

    word_data nf2 = {};
    nf2.set_prio_nf(2);

    // This tests priorities because e.g. spec1 vs ichi1, one lacks
    // spec and one lacks ichi, but spec1 becomes more common than ichi1,
    // because spec is compared first.
    REQUIRE(spec1 == spec1);
    REQUIRE(spec1 < ichi1);
    REQUIRE(spec1 < news1);
    REQUIRE(spec1 < spec2);
    REQUIRE(spec1 < ichi2);
    REQUIRE(spec1 < news2);
    REQUIRE(spec1 < nf1);
    REQUIRE(spec1 < nf2);

    REQUIRE(ichi1 > spec1);
    REQUIRE(ichi1 == ichi1);
    REQUIRE(ichi1 < news1);
    REQUIRE(ichi1 < spec2);
    REQUIRE(ichi1 < ichi2);
    REQUIRE(ichi1 < news2);
    REQUIRE(ichi1 < nf1);
    REQUIRE(ichi1 < nf2);

    REQUIRE(news1 > spec1);
    REQUIRE(news1 > ichi1);
    REQUIRE(news1 == news1);
    REQUIRE(news1 < spec2);
    REQUIRE(news1 < ichi2);
    REQUIRE(news1 < news2);
    REQUIRE(news1 < nf1);
    REQUIRE(news1 < nf2);

    REQUIRE(spec2 > spec1);
    REQUIRE(spec2 > ichi1);
    REQUIRE(spec2 > news1);
    REQUIRE(spec2 == spec2);
    REQUIRE(spec2 < ichi2);
    REQUIRE(spec2 < news2);
    REQUIRE(spec2 < nf1);
    REQUIRE(spec2 < nf2);

    REQUIRE(ichi2 > spec1);
    REQUIRE(ichi2 > ichi1);
    REQUIRE(ichi2 > news1);
    REQUIRE(ichi2 > spec2);
    REQUIRE(ichi2 == ichi2);
    REQUIRE(ichi2 < news2);
    REQUIRE(ichi2 < nf1);
    REQUIRE(ichi2 < nf2);

    REQUIRE(news2 > spec1);
    REQUIRE(news2 > ichi1);
    REQUIRE(news2 > news1);
    REQUIRE(news2 > spec2);
    REQUIRE(news2 > ichi2);
    REQUIRE(news2 == news2);
    REQUIRE(news2 < nf1);
    REQUIRE(news2 < nf2);

    REQUIRE(nf1 > spec1);
    REQUIRE(nf1 > ichi1);
    REQUIRE(nf1 > news1);
    REQUIRE(nf1 > spec2);
    REQUIRE(nf1 > ichi2);
    REQUIRE(nf1 > news2);
    REQUIRE(nf1 == nf1);
    REQUIRE(nf1 < nf2);

    REQUIRE(nf2 > spec1);
    REQUIRE(nf2 > ichi1);
    REQUIRE(nf2 > news1);
    REQUIRE(nf2 > spec2);
    REQUIRE(nf2 > ichi2);
    REQUIRE(nf2 > news2);
    REQUIRE(nf2 > nf1);
    REQUIRE(nf2 == nf2);

    // nf overrides other prios, but not until both have nf, since the other
    // prio indicators by their mere existence indicate some commonness.
    std::vector<std::function<void(word_data&, std::optional<int>)>> set_prio_functions =
    {
        [](word_data& w, std::optional<int> prio) { w.set_prio_spec(prio); },
        [](word_data& w, std::optional<int> prio) { w.set_prio_ichi(prio); },
        [](word_data& w, std::optional<int> prio) { w.set_prio_news(prio); }
    };

    for (auto set_prio : set_prio_functions) {
        word_data w1 = {};
        word_data w2 = {};

        set_prio(w1, 1);
        set_prio(w2, 2);

        REQUIRE(w1 < w2);
        w1.set_prio_nf(10);
        REQUIRE(w1 < w2);
        w2.set_prio_nf(5);
        REQUIRE(w1 > w2);
    }
}

#endif
