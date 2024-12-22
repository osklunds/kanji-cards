
#include "catch.hpp"
#include <iostream>

#include "word_data.hpp"

#ifdef TEST

TEST_CASE("word_data_sample_data") {
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file("../data/JMdict_e_sample.xml");
    REQUIRE(result == true);

    std::vector<word_data> word_datas = word_data::read_from_doc(doc, "物");

    for (word_data word_data : word_datas) {
        std::cout << word_data.as_string() << std::endl;
    }
}

#endif
