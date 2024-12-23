
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
        word_data word_data {};
        pugi::xml_node entry = xpath_entry.node();

        pugi::xml_node k_ele = entry.child("k_ele");
        pugi::xml_node keb = k_ele.child("keb");
        word_data.word = keb.text().get();

        pugi::xml_node r_ele = entry.child("r_ele");
        pugi::xml_node reb = r_ele.child("reb");
        word_data.reading = reb.text().get();

        for (auto sense : entry.children("sense")) {
            for (auto meaning : sense.children("gloss")) {
                word_data.meanings.push_back(meaning.text().get());
            }
        }

        for (auto ke_pri : k_ele.children("ke_pri")) {
            std::string text = ke_pri.text().get();
            if (text.find("news") != std::string::npos) {
                word_data.prio_news = std::stoi(text.substr(4, 100));
            }
            if (text.find("ichi") != std::string::npos) {
                word_data.prio_ichi = std::stoi(text.substr(4, 100));
            }
            if (text.find("spec") != std::string::npos) {
                word_data.prio_spec = std::stoi(text.substr(4, 100));
            }
            if (text.find("nf") != std::string::npos) {
                word_data.prio_nf = std::stoi(text.substr(2, 100));
            }
        }

        word_datas.push_back(word_data);
    }

    return word_datas;
}

std::string word_data::get_word() {
    return word;
}

std::vector<std::string> word_data::get_meanings() {
    return meanings;
}

std::string word_data::get_reading() {
    return reading;
}

std::optional<int> word_data::get_prio_news() {
    return prio_news;
}

std::optional<int> word_data::get_prio_ichi() {
    return prio_ichi;
}

std::optional<int> word_data::get_prio_spec() {
    return prio_spec;
}

std::optional<int> word_data::get_prio_nf() {
    return prio_nf;
}

std::string word_data::as_string() {
    std::string string {};

    string += "Word: " + word + "\n";
    string += "Reading: " + reading + "\n";
    string += "Meanings: ";
    for (auto meaning : meanings) {
        string += meaning + ", ";
    }
    string.pop_back();
    string.pop_back();
    string += "\n";

    auto to_string = [](std::optional<int> prio) {
        return prio.transform([](int prio) {
            return std::to_string(prio);
        }).value_or("");
    };

    string += "Prio news: ";
    string += to_string(prio_news);
    string += "\n";

    string += "Prio ichi: ";
    string += to_string(prio_ichi);
    string += "\n";

    string += "Prio spec: ";
    string += to_string(prio_spec);
    string += "\n";

    string += "Prio nf: ";
    string += to_string(prio_nf);

    return string;
}
