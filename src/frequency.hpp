
#ifndef HEADER_FREQUENCY_HPP
#define HEADER_FREQUENCY_HPP

#include <vector>

class frequency_entry {
private:
    std::string kanji;
    int frequency;

public:
    frequency_entry(const std::string& kanji, int frequency);

    int get_frequency() const;
    const std::string& get_kanji() const;

    friend bool operator< (const frequency_entry& fe1, const frequency_entry& fe2);
};

std::vector<frequency_entry> get_frequency_entries_from_file(const std::string& file_path);

#endif

