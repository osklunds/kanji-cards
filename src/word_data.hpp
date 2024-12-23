
#ifndef HEADER_WORD_DATA_HPP
#define HEADER_WORD_DATA_HPP

#include <vector>
#include <string>
#include <optional>
#include "pugixml.hpp"

class word_data {
private:
    std::string word {};
    std::vector<std::string> meanings {};
    std::string reading {};

    std::optional<int> prio_news {};
    std::optional<int> prio_ichi {};
    std::optional<int> prio_spec {};
    std::optional<int> prio_nf {};

    word_data();

public:
    static std::vector<word_data> read_from_doc(pugi::xml_document& doc,
                                                std::string kanji
                                                );

    std::string get_word();
    std::vector<std::string> get_meanings();
    std::string get_reading();

    std::optional<int> get_prio_news();
    std::optional<int> get_prio_ichi();
    std::optional<int> get_prio_spec();
    std::optional<int> get_prio_nf();

    std::string as_string();
};

#endif
