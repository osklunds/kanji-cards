
#include <iostream>
#include <cassert>
#include <cstring>
#include <format>
#include "pugixml.hpp"

#include "kanji_data.hpp"

kanji_data::kanji_data(std::string kanji) :
    kanji { kanji },
    meanings {}
{

}

bool kanji_data::read_from_file(std::string path) {
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file(path.c_str());
    assert(result == true);

    std::string xpath =
        std::format("/kanjidic2/character[./literal = \"{}\"]", this->kanji);
    pugi::xpath_node xpath_character_node = doc.select_node(xpath.c_str());
    pugi::xml_node character_node = xpath_character_node.node();

    pugi::xml_node literal_node = character_node.child("literal");

    const pugi::char_t *text = literal_node.text().get();
    assert(strcmp(text, this->kanji.c_str()) == 0);

    return true;
}
