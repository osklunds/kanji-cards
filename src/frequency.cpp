
#include <iostream>
#include <regex>
#include <string>
#include <cassert>
#include <fstream>

#include "frequency.hpp"

frequency_entry::frequency_entry(std::string kanji, int frequency) {
    this->kanji = kanji;
    this->frequency = frequency;
}

int frequency_entry::get_frequency() {
    return frequency;
}

std::string frequency_entry::get_kanji() {
    return kanji;
}

std::vector<frequency_entry> get_frequency_entries_from_file(std::string file_path) {
    std::ifstream file(file_path);
    assert(file.is_open());

    std::vector<frequency_entry> entries;

    std::string line;
    while (std::getline(file, line)) {
        std::regex regex("(.*),(\\d+,){9}(\\d+)");
        std::smatch match;
        std::regex_search(line, match, regex);

        if (!match.empty()) {
            assert(match.size() == 4);
            std::string kanji = match[1];
            int frequency = std::stoi(match[3]);
            frequency_entry entry(kanji, frequency);
            entries.push_back(entry);
        }
    }
    file.close();

    std::sort(entries.begin(), entries.end());

    return entries;
}

bool operator< (const frequency_entry& fe1, const frequency_entry& fe2) {
    if (fe1.frequency == fe2.frequency) {
        return fe1.kanji < fe2.kanji;
    } else {
        return fe1.frequency < fe2.frequency;
    }
}

