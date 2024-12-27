
#include "stroke_order.hpp"

#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <filesystem>

TEST_CASE("path_for_kanji") {
    std::string path = path_for_kanji("04fd7");
    REQUIRE("../data/kanji/04fd7.svg" == path);
    REQUIRE(std::filesystem::exists(path));
}
