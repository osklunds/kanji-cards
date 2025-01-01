
/*
  word_data contains all the info that is displayed per word. Based on the XML
  document JMDict_e, word_data for all words containing a specified kanji are
  returned, sorted by freqency.
 */

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

    int prio_mapped() const;

public:
    static std::vector<word_data> read_from_doc(const pugi::xml_document& doc,
                                                const std::string &kanji
                                                );

    word_data();

    const std::string& get_word() const;
    void set_word(const std::string& word);
    
    const std::vector<std::string>& get_meanings() const;
    const std::string& get_reading() const;

    const std::optional<int>& get_prio_news() const;
    void set_prio_news(const std::optional<int>& prio_news);
    
    const std::optional<int>& get_prio_ichi() const;
    void set_prio_ichi(const std::optional<int>& prio_ichi);

    const std::optional<int>& get_prio_spec() const;
    void set_prio_spec(const std::optional<int>& prio_spec);

    const std::optional<int>& get_prio_nf() const;
    void set_prio_nf(const std::optional<int>& prio_nf);

    std::string as_string() const;
    std::string as_pretty_string() const;

    friend std::ostream& operator<< (std::ostream& os, const word_data& w);
    friend bool operator== (const word_data& w1, const word_data& w2);
    friend std::weak_ordering operator<=> (const word_data& w1, const word_data& w2);
};

std::ostream& operator<< (std::ostream& os, const word_data& w);
bool operator== (const word_data& w1, const word_data& w2);
std::weak_ordering operator<=> (const word_data& w1, const word_data& w2);

#endif
