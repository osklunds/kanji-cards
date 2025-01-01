
/*
  kanji_data contains all the info that is displayed per kanji. Based on the XML
  documents kanjidic2 and JMDict_e, all info is retrieved for a specified kanji.
 */

#ifndef HEADER_KANJI_DATA_HPP
#define HEADER_KANJI_DATA_HPP

#include <vector>
#include <string>
#include <cstdint>
#include "pugixml.hpp"
#include "word_data.hpp"

class kanji_data {
private:
    std::string kanji {};
    uint16_t frequency {};
    std::vector<std::string> meanings {};
    std::vector<std::string> kun_readings {};
    std::vector<std::string> on_readings {};
    std::vector<word_data> words {};
    std::vector<std::vector<uint8_t>> stroke_order_jpgs {};

    std::string vector_as_string(const std::string& prefix,
                                 const std::vector<std::string>& strings
                                 ) const;

public:
    kanji_data(const std::string& kanji,
               uint16_t frequency,
               const pugi::xml_document& kanjidic2_doc,
               const pugi::xml_document& jmdict_e_doc
               );

    const std::string& get_kanji() const;
    uint16_t get_frequency() const;
    const std::vector<std::string>& get_meanings() const;
    const std::vector<std::string>& get_kun_readings() const;
    const std::vector<std::string>& get_on_readings() const;
    const std::vector<word_data>& get_words() const;
    const std::vector<std::vector<uint8_t>>& get_stroke_order_jpgs() const;

    std::string as_string() const;
    std::string as_pretty_string() const;
};

#endif
