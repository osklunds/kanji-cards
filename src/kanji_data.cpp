
#include <iostream>
#include <cassert>
#include <cstring>
#include <format>

#include "kanji_data.hpp"

kanji_data::kanji_data(std::string kanji) :
    kanji { kanji },
    meanings {},
    kun_readings {},
    on_readings {}
{

}

void kanji_data::read_from_doc(pugi::xml_document& doc) {
    std::string xpath =
        std::format("/kanjidic2/character[./literal = \"{}\"]", this->kanji);
    pugi::xpath_node xpath_character_node = doc.select_node(xpath.c_str());
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

std::string kanji_data::as_string() {
    std::string string {};

    string += "Kanji: " + kanji + "\n";

    string += vector_as_string("Meanings", this->meanings);
    string += vector_as_string("Kun readings", this->kun_readings);
    string += vector_as_string("On readings", this->on_readings);

    string.pop_back();

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
