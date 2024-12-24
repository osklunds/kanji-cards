
#ifndef HEADER_WORD_DATA_HPP
#define HEADER_WORD_DATA_HPP

#include <vector>
#include <string>
#include <optional>
#include <compare>
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

public:
    static std::vector<word_data> read_from_doc(pugi::xml_document& doc,
                                                std::string kanji
                                                );

    word_data();

    std::string get_word();
    void set_word(std::string word);
    
    std::vector<std::string> get_meanings();

    std::string get_reading();

    std::optional<int> get_prio_news();
    void set_prio_news(std::optional<int> prio_news);
    
    std::optional<int> get_prio_ichi();
    void set_prio_ichi(std::optional<int> prio_ichi);

    std::optional<int> get_prio_spec();
    void set_prio_spec(std::optional<int> prio_spec);

    std::optional<int> get_prio_nf();
    void set_prio_nf(std::optional<int> prio_nf);

    std::string as_string();

    friend std::weak_ordering operator<=> (const word_data& w1, const word_data& w2);
};

std::weak_ordering operator<=> (const word_data& w1, const word_data& w2);

#endif
