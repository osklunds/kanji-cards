
#include "catch.hpp"
#include <iostream>

#include "word_data.hpp"

#ifdef TEST

TEST_CASE("word_data_sample_data") {
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

#endif
