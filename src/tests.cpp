
#include <iostream>
#include "catch.hpp"

#ifdef TEST

TEST_CASE("hej") {
    std::cout << "hejjjj" << std::endl;
    REQUIRE(1+1 == 2);
}

#endif
