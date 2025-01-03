
// Copyright (C) 2025 Oskar Lundström

// This file is part of kanji-cards.

// kanji-cards is free software: you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free Software
// Foundation, either version 3 of the License, or (at your option) any later
// version.

// kanji-cards is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
// FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
// details.

// You should have received a copy of the GNU General Public License along with
// kanji-cards. If not, see <https://www.gnu.org/licenses/>.

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
