
#include <iostream>
#include <cassert>
#include <cstring>
#include <format>

#include "kanji_data.hpp"
#include "stroke_order.hpp"

kanji_data::kanji_data(const std::string& kanji,
                       uint16_t frequency,
                       const pugi::xml_document& kanjidic2_doc,
                       const pugi::xml_document& jmdict_e_doc
                       )  :
    kanji { kanji },
    frequency { frequency },
    meanings {},
    kun_readings {},
    on_readings {},
    words {},
    stroke_order_jpgs {}
{
    std::string xpath =
        std::format("/kanjidic2/character[./literal = \"{}\"]", this->kanji);
    pugi::xpath_node xpath_character_node = kanjidic2_doc.select_node(xpath.c_str());
    pugi::xml_node character_node = xpath_character_node.node();

    pugi::xml_node literal_node = character_node.child("literal");
    const pugi::char_t *text = literal_node.text().get();
    assert(strcmp(text, this->kanji.c_str()) == 0);

    pugi::xml_node reading_meaning = character_node.child("reading_meaning");
    pugi::xml_node rmgroup = reading_meaning.child("rmgroup");

    for (auto meaning : rmgroup.children("meaning")) {
        if (meaning.first_attribute().empty()) {
            this->meanings.push_back(meaning.text().get());
        }
    }

    for (auto reading : rmgroup.children("reading")) {
        if (strcmp(reading.attribute("r_type").value(), "ja_kun") == 0) {
            this->kun_readings.push_back(reading.text().get());
        } else if (strcmp(reading.attribute("r_type").value(), "ja_on") == 0) {
            this->on_readings.push_back(reading.text().get());
        }
    }

    pugi::xml_node codepoint = character_node.child("codepoint");
    for (auto cp_value : codepoint.children("cp_value")) {
        if (strcmp(cp_value.attribute("cp_type").value(), "ucs") == 0) {
            std::string code_point = cp_value.text().get();
            stroke_order_jpgs = code_point_to_stroke_order_jpgs(code_point);
        }
    }
    assert(!stroke_order_jpgs.empty());

    this->words = word_data::read_from_doc(jmdict_e_doc, this->kanji);
}

const std::string& kanji_data::get_kanji() const {
    return kanji;
}

uint16_t kanji_data::get_frequency() const {
    return frequency;
}

const std::vector<std::string>& kanji_data::get_meanings() const {
    return meanings;
}

const std::vector<std::string>& kanji_data::get_kun_readings() const {
    return kun_readings;
}

const std::vector<std::string>& kanji_data::get_on_readings() const {
    return on_readings;
}

const std::vector<word_data>& kanji_data::get_words() const {
    return words;
}

const std::vector<std::vector<uint8_t>>& kanji_data::get_stroke_order_jpgs() const {
    return stroke_order_jpgs;
}

std::string kanji_data::as_string() const {
    std::string string {};

    string += "-----\nKanji: " + kanji + "\n";

    string += vector_as_string("Meanings", meanings);
    string += vector_as_string("Kun readings", kun_readings);
    string += vector_as_string("On readings", on_readings);

    for (word_data word_data : words) {
        string += "---\n";
        string += word_data.as_string();
        string += "\n";
    }
    string += "-----";

    return string;
}

std::string kanji_data::vector_as_string(const std::string& prefix,
                                         const std::vector<std::string>& strings
                                         ) const {
    std::string string {};

    string += prefix + ": ";
    for (auto str : strings) {
        string += str + ", ";
    }
    string.pop_back();
    string.pop_back();
    string += "\n";

    return string;
}

std::string kanji_data::as_pretty_string() const {
    std::string string {};

    string += "Kanji: " + kanji + "\n";

    string += vector_as_string("Meanings", meanings);
    string += vector_as_string("Kun readings", kun_readings);
    string += vector_as_string("On readings", on_readings);

    auto word_data_it = words.begin();
    for (int i = 0; i < 5; i++) {
        string += word_data_it->get_word() + " (" + word_data_it->get_reading() + ") ";

        std::vector<std::string> word_meanings = word_data_it->get_meanings();
        auto meaning_it = word_meanings.begin();
        for (int j = 0; j < 3; j++) {
            string += *meaning_it + ", ";
            meaning_it++;
            if (meaning_it == word_meanings.end()) {
                break;
            }
        }
        string.pop_back();
        string.pop_back();

        string += "\n";

        word_data_it++;
        if (word_data_it == words.end()) {
            break;
        }
    }

    return string;
}
