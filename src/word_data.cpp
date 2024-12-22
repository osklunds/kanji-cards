
#include <iostream>
#include <cassert>
#include <cstring>
#include <format>

#include "word_data.hpp"

word_data::word_data() :
    word {},
    meanings {},
    reading {},
    prio_news {},
    prio_ichi {},
    prio_spec {},
    prio_nf {}
{

}

std::vector<word_data> word_data::read_from_doc(pugi::xml_document& doc,
                                                       std::string kanji
                                                       ) {
    std::string xpath =
        std::format("/JMdict/entry[contains(./k_ele/keb,\"{}\")]", kanji);
    pugi::xpath_node_set xpath_entries = doc.select_nodes(xpath.c_str());

    std::vector<word_data> word_datas {};

    for (pugi::xpath_node xpath_entry : xpath_entries) {
        pugi::xml_node entry = xpath_entry.node();
        pugi::xml_node k_ele = entry.child("k_ele");
        pugi::xml_node keb = k_ele.child("keb");

        word_data word_data {};
        word_data.word = keb.text().get();

        for (auto sense : entry.children("sense")) {
            for (auto meaning : sense.children("gloss")) {
                word_data.meanings.push_back(meaning.text().get());
            }
        }

        for (auto ke_pri : k_ele.children("ke_pri")) {
            auto text = ke_pri.text().get();
            // std::cout << "oskar: " << text << std::endl;
        }

        word_datas.push_back(word_data);
    }

    return word_datas;
}

std::string word_data::as_string() {
    return this->word;
}
