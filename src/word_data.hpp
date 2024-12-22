
#ifndef HEADER_WORD_DATA_HPP
#define HEADER_WORD_DATA_HPP

#include <vector>
#include <string>
#include "pugixml.hpp"

class word_data {
private:
    std::string word {};
    std::vector<std::string> meanings {};
    std::string reading {};

    std::string prio_news {};
    std::string prio_ichi {};
    std::string prio_spec {};
    std::string prio_nf {};

public:
    static std::vector<word_data> read_from_doc(pugi::xml_document& doc,
                                                std::string kanji
                                                );

    word_data();

    std::string as_string();
};

#endif
