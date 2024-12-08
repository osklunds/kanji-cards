
#include <iostream>
#include <string.h>
#include "catch.hpp"
#include "pugixml.hpp"

#include <cstdlib>
#define cimg_display 0
#include "CImg.h"

using namespace cimg_library;
using namespace std;

#ifdef TEST

TEST_CASE("learning") {
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file("sample.xml");
    REQUIRE(result == true);

    pugi::xml_node note = doc.child("note");
    const char *name = note.name();
    REQUIRE(strcmp(name, "note") == 0);

    pugi::xml_node child = note.child("some_tag");
    REQUIRE(strcmp(child.name(), "some_tag") == 0);

    pugi::xml_text text = child.text();
    const char *text_as_char = text.get();
    REQUIRE(strcmp(text_as_char, "Hello") == 0);

    pugi::xml_node n1 = note.first_child();
    pugi::xml_node n2 = n1.next_sibling();
    pugi::xml_node n3 = n2.next_sibling();
    pugi::xml_node n4 = n3.next_sibling();
    pugi::xml_node afterLast = n4.next_sibling();

    REQUIRE(strcmp(n1.name(), "some_tag") == 0);
    REQUIRE(strcmp(n2.name(), "first_tag") == 0);
    REQUIRE(strcmp(n3.name(), "second_tag") == 0);
    REQUIRE(strcmp(n4.name(), "third_tag") == 0);
    REQUIRE(strcmp(afterLast.name(), "") == 0);

    REQUIRE(strcmp(n1.text().get(), "Hello") == 0);
    REQUIRE(strcmp(n2.text().get(), "yo") == 0);
    REQUIRE(strcmp(n3.text().get(), "hi") == 0);
    REQUIRE(strcmp(n4.text().get(), "abc") == 0);
    REQUIRE(strcmp(afterLast.text().get(), "") == 0);

    REQUIRE(n1.empty() == false);
    REQUIRE(n2.empty() == false);
    REQUIRE(n3.empty() == false);
    REQUIRE(n4.empty() == false);
    REQUIRE(afterLast.empty() == true);
}

TEST_CASE("learning_img") {
    CImg<unsigned char> image(300,200,1,3);

    cimg_forXY(image,x,y) {
        image(x,y,0,0)=255;
        image(x,y,0,1)=255;
        image(x,y,0,2)=255;
    }

    // Make some colours
    unsigned char black[] = {0, 0, 0};
    unsigned char white[] = {255, 255, 255};

    cout << u8"hej 感じ" << endl;

    image.draw_text(30,60,u8"hejåäö 感じ",black,white,1,16);

    image.save_png("result.png");
}

#endif
