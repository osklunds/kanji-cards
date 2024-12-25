
#ifndef HEADER_KANJI_DATA_HPP
#define HEADER_KANJI_DATA_HPP

#include <vector>
#include <string>
#include "pugixml.hpp"
#include "word_data.hpp"

class kanji_data {
private:
    std::string kanji {};
    std::vector<std::string> meanings {};
    std::vector<std::string> kun_readings {};
    std::vector<std::string> on_readings {};
    std::vector<word_data> words {};

    std::string vector_as_string(std::string prefix,
                                 std::vector<std::string>& strings
                                 );

public:
    kanji_data(std::string kanji);

    void read_from_doc(pugi::xml_document& kanjidic2_doc,
                       pugi::xml_document& jmdict_e_doc
                       );

    std::vector<std::string> get_meanings();
    std::vector<std::string> get_kun_readings();
    std::vector<std::string> get_on_readings();

    std::string as_string();
};

#endif
