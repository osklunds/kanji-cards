
#include "stroke_order.hpp"

#include <cassert>

std::string path_for_kanji(const std::string& code_point) {
    assert(code_point.size() == 5);

    return "../data/kanji/" + code_point + ".svg";
}
