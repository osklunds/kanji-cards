
#include "create_card.hpp"

#include <catch2/catch_test_macros.hpp>
#include <filesystem>
#include "frequency.hpp"
#include <iostream>

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

// A convenient way to run the main function
TEST_CASE("run_main", "[.]") {
    std::vector<frequency_entry> entries =
        get_frequency_entries_from_file("../data/kanji_freqency_list.csv");
    entries.erase(entries.begin() + 20, entries.end());

    pugi::xml_document kanjidic2_doc {};
    pugi::xml_parse_result result_kanjidic2 =
        kanjidic2_doc.load_file("../data/kanjidic2.xml");
    REQUIRE(result_kanjidic2);

    pugi::xml_document jmdict_e_doc {};
    pugi::xml_parse_result result_jmdict_e =
        jmdict_e_doc.load_file("../data/JMdict_e.xml");
    REQUIRE(result_jmdict_e);

    // todo: constant for out dir
    std::string out_dir = "out";
    std::filesystem::remove_all(out_dir);
    REQUIRE(!std::filesystem::exists(out_dir));
    REQUIRE(std::filesystem::create_directory(out_dir));

    for (int i = 0; i < entries.size(); i++) {
        auto fe = entries[i];
        kanji_data kanji_data { fe.get_kanji(),
                                static_cast<uint16_t>(i + 1),
                                kanjidic2_doc,
                                jmdict_e_doc
        };

        create_card(kanji_data, out_dir);
        std::cout << "Created: " << fe.get_kanji() << std::endl;
    }
}
