
#include "util.hpp"

#include <cassert>
#include <iostream>
#include <memory>
#include <fstream>

// Inspired by https://gist.github.com/meritozh/f0351894a2a4aa92871746bf45879157
std::vector<uint8_t> exec(std::string cmd) {
    std::shared_ptr<FILE> pipe { popen(cmd.c_str(), "r"), pclose };
    assert(pipe);

    std::vector<uint8_t> result {};

    while (!feof(pipe.get())) {
        result.push_back(fgetc(pipe.get()));
    }

    // For some reason, 'm' is added in the end
    result.pop_back();

    return result;
}

std::vector<uint8_t> read_file_bin(std::string path) {
    std::ifstream input { path.c_str(), std::ios::binary };
    std::vector<uint8_t> buffer(std::istreambuf_iterator<char>(input), {});
    return buffer;
}
