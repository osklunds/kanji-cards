
#include "util.hpp"

#include <catch2/catch_test_macros.hpp>

TEST_CASE("exec") {
    // Compare exec with known output
    REQUIRE(exec("echo hej") == (std::vector<uint8_t>){'h', 'e', 'j', '\n'});

    // Compare exec and read_file_bin for text file
    REQUIRE(exec("cat JMdict_e.xml.sample") == read_file_bin("JMdict_e.xml.sample"));

    // Compare exec and read_file_bin for binary file
    auto binResult = exec("cat main");
    REQUIRE(binResult == read_file_bin("main"));
    // Safety check to see that it's indeed a big file
    REQUIRE(binResult.size() > 1000*1000);
}
