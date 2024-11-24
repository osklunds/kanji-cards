
#include <iostream>
#include "catch.hpp"

TEST_CASE("hej") {
    std::cout << "Hello Worldd!";
    REQUIRE(1+1 == 2);
}
