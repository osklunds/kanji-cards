
#include "create_card.hpp"

#include <catch2/catch_test_macros.hpp>

TEST_CASE("create_card") {
    pugi::xml_document kanjidic2_doc {};
    pugi::xml_parse_result result_kanjidic2 =
        kanjidic2_doc.load_file("../data/kanjidic2_sample.xml");
    REQUIRE(result_kanjidic2 == true);

    pugi::xml_document jmdict_e_doc {};
    pugi::xml_parse_result result_jmdict_e =
        jmdict_e_doc.load_file("../data/JMdict_e_sample.xml");
    REQUIRE(result_jmdict_e == true);

    kanji_data kanji_data { "日", kanjidic2_doc, jmdict_e_doc };

    create_card(kanji_data, "create_card.pdf");
}
