
#include "catch.hpp"
#include <string>

#include "kanji_data.hpp"

#ifdef TEST

TEST_CASE("test") {
    kanji_data kanji_data { "日" };
    kanji_data.read_from_file("../data/kanjidic2.xml");
}

#endif
