
#ifndef HEADER_FREQUENCY_HPP
#define HEADER_FREQUENCY_HPP

#include <vector>
#include <string>

class kanji_data {
private:
    std::string kanji {};
    std::vector<std::string> meanings {};

public:
    kanji_data(std::string kanji);

    bool read_from_file(std::string path);

    std::vector<std::string> get_meanings();
};

#endif
