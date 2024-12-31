
#include "create_card.hpp"

#include <catch2/catch_test_macros.hpp>
#include <filesystem>

TEST_CASE("create_card") {
    pugi::xml_document kanjidic2_doc {};
    pugi::xml_parse_result result_kanjidic2 =
        kanjidic2_doc.load_file("../data/kanjidic2.xml");
    REQUIRE(result_kanjidic2 == true);

    pugi::xml_document jmdict_e_doc {};
    pugi::xml_parse_result result_jmdict_e =
        jmdict_e_doc.load_file("../data/JMdict_e.xml");
    REQUIRE(result_jmdict_e == true);

    // 生 has lots of words and words wrapping lines
    kanji_data kanji_data1 { "生", 1, kanjidic2_doc, jmdict_e_doc };

    // 働 has multiple lines of stroker order graphs
    kanji_data kanji_data2 { "働", 1234, kanjidic2_doc, jmdict_e_doc };

    std::string out_dir = "out";
    std::filesystem::remove_all(out_dir);
    REQUIRE(!std::filesystem::exists(out_dir));
    REQUIRE(std::filesystem::create_directory(out_dir));

    std::string kanji1 = out_dir + "/0001-生.pdf";
    std::string kanji2 = out_dir + "/1234-働.pdf";

    REQUIRE(std::filesystem::exists(out_dir));
    REQUIRE(!std::filesystem::exists(kanji1));
    REQUIRE(!std::filesystem::exists(kanji2));

    create_card(kanji_data1, out_dir);
    create_card(kanji_data2, out_dir);

    REQUIRE(std::filesystem::exists(out_dir));
    REQUIRE(std::filesystem::exists(kanji1));
    REQUIRE(std::filesystem::exists(kanji2));
}
