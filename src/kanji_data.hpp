
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
               const pugi::xml_document& kanjidic2_doc,
               const pugi::xml_document& jmdict_e_doc
               );

    const std::string& get_kanji() const;
    const std::vector<std::string>& get_meanings() const;
    const std::vector<std::string>& get_kun_readings() const;
    const std::vector<std::string>& get_on_readings() const;
    const std::vector<word_data>& get_words() const;
    const std::vector<std::vector<uint8_t>>& get_stroke_order_jpgs() const;

    std::string as_string() const;
    std::string as_pretty_string() const;
};

#endif
