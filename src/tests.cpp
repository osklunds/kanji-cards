
#include <iostream>
#include "pugixml.hpp"
#define CATCH_CONFIG_MAIN
#include "catch.hpp"

TEST_CASE("hej") {
    std::cout << "Hello World!";
    REQUIRE(1+1 == 2);
}
