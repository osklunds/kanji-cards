
#include <iostream>
#include <cassert>
#include <cstring>
#include <format>
#include <limits.h>
#include <cmath>

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

std::vector<word_data> word_data::read_from_doc(const pugi::xml_document& doc,
                                                const std::string& kanji
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

    std::sort(word_datas.begin(), word_datas.end());

    return word_datas;
}

const std::string& word_data::get_word() const {
    return word;
}

void word_data::set_word(const std::string& word) {
    this->word = word;
}

const std::vector<std::string>& word_data::get_meanings() const {
    return meanings;
}

const std::string& word_data::get_reading() const {
    return reading;
}

const std::optional<int>& word_data::get_prio_news() const {
    return prio_news;
}

void word_data::set_prio_news(const std::optional<int>& prio_news) {
    this->prio_news = prio_news;
}

const std::optional<int>& word_data::get_prio_ichi() const {
    return prio_ichi;
}

void word_data::set_prio_ichi(const std::optional<int>& prio_ichi) {
    this->prio_ichi = prio_ichi;
}

const std::optional<int>& word_data::get_prio_spec() const {
    return prio_spec;
}

void word_data::set_prio_spec(const std::optional<int>& prio_spec) {
    this->prio_spec = prio_spec;
}

const std::optional<int>& word_data::get_prio_nf() const {
    return prio_nf;
}

void word_data::set_prio_nf(const std::optional<int>& prio_nf) {
    this->prio_nf = prio_nf;
}

std::string word_data::as_string() const {
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

std::ostream& operator<< (std::ostream& os, const word_data& w) {
    os << w.as_string();
    return os;
}

bool operator== (const word_data& w1, const word_data& w2) {
    return w1 <=> w2 == std::weak_ordering::equivalent;
}

std::weak_ordering operator<=> (const word_data& w1, const word_data& w2) {
    return w1.prio_mapped() <=> w2.prio_mapped();
}

int word_data::prio_mapped() const {
    // Number of words with these tags in JMDict_e.xml
    const int num_news1_words = 23356;
    const int num_news2_words = 21618;
    const int num_ichi1_words = 18487;
    const int num_ichi2_words = 168;
    const int num_spec1_words = 3391;
    const int num_spec2_words = 3316;

    // The idea is, assume that if a word has tag news1, it is among
    // the 23356 most common. Assume it is in the middle, i.e. the 23356/2
    // most common word. Then divide by 500 to translate to nf group.
    auto num_words_to_nf_group = [](int num_words) {
        const int num_words_per_nf_group = 500;
        return std::round((double)num_words / (double)(num_words_per_nf_group*2));
    };

    int mapped_news = INT_MAX;
    if (prio_news == 1) {
        mapped_news = num_words_to_nf_group(num_news1_words);
    } else if (prio_news == 2) {
        mapped_news = num_words_to_nf_group(num_news1_words + num_news2_words);
    }

    int mapped_ichi = INT_MAX;
    if (prio_ichi == 1) {
        mapped_ichi = num_words_to_nf_group(num_ichi1_words);
    } else if (prio_ichi == 2) {
        mapped_ichi = num_words_to_nf_group(num_ichi1_words + num_ichi2_words);
    }

    int mapped_spec = INT_MAX;
    if (prio_spec == 1) {
        mapped_spec = num_words_to_nf_group(num_spec1_words);
    } else if (prio_spec == 2) {
        mapped_spec = num_words_to_nf_group(num_spec1_words + num_spec2_words);
    }

    int mapped_nf = INT_MAX;
    if (prio_nf.has_value()) {
        mapped_nf = prio_nf.value();
    }

    std::vector<int> mapped_prios = {
        mapped_nf,
        mapped_news,
        mapped_ichi,
        mapped_spec
    };

    return *std::min_element(mapped_prios.begin(), mapped_prios.end());
}
