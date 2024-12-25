
#include <iostream>
#include <cassert>
#include <cstring>
#include <format>

#include "kanji_data.hpp"

kanji_data::kanji_data(std::string kanji) :
    kanji { kanji },
    meanings {},
    kun_readings {},
    on_readings {},
    words {}
{

}

void kanji_data::read_from_doc(pugi::xml_document& kanjidic2_doc,
                               pugi::xml_document& jmdict_e_doc
                               ) {
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

    this->words = word_data::read_from_doc(jmdict_e_doc, this->kanji);
}

std::string kanji_data::get_kanji() {
    return kanji;
}

std::vector<std::string> kanji_data::get_meanings() {
    return this->meanings;
}

std::vector<std::string> kanji_data::get_kun_readings() {
    return this->kun_readings;
}

std::vector<std::string> kanji_data::get_on_readings() {
    return this->on_readings;
}

std::vector<word_data> kanji_data::get_words() {
    return words;
}

std::string kanji_data::as_string() {
    std::string string {};

    string += "-----\nKanji: " + kanji + "\n";

    string += vector_as_string("Meanings", this->meanings);
    string += vector_as_string("Kun readings", this->kun_readings);
    string += vector_as_string("On readings", this->on_readings);

    for (word_data word_data : words) {
        string += "---\n";
        string += word_data.as_string();
        string += "\n";
    }
    string += "-----";

    return string;
}

std::string kanji_data::vector_as_string(std::string prefix,
                                         std::vector<std::string>& strings
                                         ) {
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

std::string kanji_data::as_pretty_string() {
    std::string string {};

    string += "Kanji: " + kanji + "\n";

    string += vector_as_string("Meanings", meanings);
    string += vector_as_string("Kun readings", kun_readings);
    string += vector_as_string("On readings", on_readings);

    auto word_data_it = words.begin();
    for (int j = 0; j < 5; j++) {
        string += word_data_it->get_word() + " (" + word_data_it->get_reading() + ") ";

        std::vector<std::string> word_meanings = word_data_it->get_meanings();
        auto it = word_meanings.begin();
        for (int i = 0; i < 3; i++) {
            string += *it + ", ";
            it++;
            if (it == word_meanings.end()) {
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
