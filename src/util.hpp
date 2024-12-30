
#ifndef HEADER_UTIL_HPP
#define HEADER_UTIL_HPP

#include <vector>
#include <string>
#include <cstdint>

std::vector<uint8_t> exec(std::string cmd);
std::vector<uint8_t> read_file_bin(std::string path);

bool regex_matches(std::string subject, std::string regex);

#endif
