
#ifndef HEADER_FREQUENCY_HPP
#define HEADER_FREQUENCY_HPP

#include <vector>
#include <string>
#include "pugixml.hpp"

class kanji_data {
private:
    std::string kanji {};
    std::vector<std::string> meanings {};
    std::vector<std::string> kun_readings {};
    std::vector<std::string> on_readings {};

public:
    kanji_data(std::string kanji);

    void read_from_file(pugi::xml_document& doc);

    std::vector<std::string> get_meanings();
    std::vector<std::string> get_kun_readings();
    std::vector<std::string> get_on_readings();
};

#endif
