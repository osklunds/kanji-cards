
#include <iostream>
#include <string>
#include <string.h>
#include <fstream>
#include <sstream>
#include <regex>

#include <catch2/catch_test_macros.hpp>
#include "pugixml.hpp"
#include <hpdf.h>

#include "frequency.hpp"
#include "kanji_data.hpp"
#include "stroke_order.hpp"
#include "util.hpp"

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

// TEST_CASE("learning_img") {
//     CImg<unsigned char> image(300,200,1,3);

//     cimg_forXY(image,x,y) {
//         image(x,y,0,0)=255;
//         image(x,y,0,1)=255;
//         image(x,y,0,2)=255;
//     }

//     // Make some colours
//     unsigned char black[] = {0, 0, 0};
//     unsigned char white[] = {255, 255, 255};

//     cout << u8"hej 感じ" << endl;

//     image.draw_text(30,60,u8"hejåäö 感じ",black,white,1,16);

//     image.save_png("result.png");
// }

TEST_CASE("learning_kanjidic2") {
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file("kanjidic2.xml");
    REQUIRE(result == true);

    pugi::xpath_node xpath_node = doc.select_node("/kanjidic2/character/literal");
    pugi::xml_node node = xpath_node.node();

    // cout << node.name() << endl;
    // cout << node.text().get() << endl;

    const pugi::char_t *text = node.text().get();
    REQUIRE(strcmp(text, "亜") == 0);
}

TEST_CASE("learning_lookup_by_kanji") {
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file("kanjidic2.xml");
    REQUIRE(result == true);

    pugi::xpath_node xpath_character_node =
        doc.select_node("/kanjidic2/character[./literal = \"日\"]");
    pugi::xml_node character_node = xpath_character_node.node();

    // std::cout << "name: " << character_node.name() << std::endl;

    pugi::xml_node literal_node = character_node.child("literal");
    // std::cout << "name: " << literal_node.name() << std::endl;
    // std::cout << "text: " << literal_node.text().get() << std::endl;

    const pugi::char_t *text = literal_node.text().get();
    REQUIRE(strcmp(text, "日") == 0);
}

TEST_CASE("learning_frequency_list") {
    std::string line;
    std::ifstream file ("../data/kanji_freqency_list.csv");
    REQUIRE(file.is_open());

    int count = 0;

    while (std::getline(file, line) && count < 5) {
        count++;
        // std::cout << line << std::endl;

        std::regex r("(.*),(\\d+,){9}(\\d+)");
        std::smatch m;
        std::regex_search(line, m, r);

        if (!m.empty()) {
            REQUIRE(m.size() == 4);

            std::string kanji = m[1];
            std::string frequency = m[3];

            // std::cout << "oskar: " << kanji << std::endl;
            // std::cout << "oskar: " << frequency << std::endl;
        }
    }
    file.close();
}

TEST_CASE("learning_JMdict_e") {
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file("JMdict_e.xml");
    REQUIRE(result == true);

    pugi::xpath_node xpath_entry = doc.select_node("/JMdict/entry[./k_ele/keb = \"竹馬\"]");
    pugi::xml_node entry = xpath_entry.node();
    pugi::xml_node k_ele = entry.child("k_ele");
    pugi::xml_node keb = k_ele.child("keb");

    // std::cout << "oskar: " << keb.name() << std::endl;
    // std::cout << "oskar: " << keb.text().get() << std::endl;

    REQUIRE(strcmp(keb.text().get(), "竹馬") == 0);
}

TEST_CASE("learning_JMdict_e_words_with_kanji") {
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file("JMdict_e.xml");
    REQUIRE(result == true);

    pugi::xpath_node_set xpath_entries =
        doc.select_nodes("/JMdict/entry[contains(./k_ele/keb,\"日\")]");

    for (pugi::xpath_node xpath_entry : xpath_entries) {
        pugi::xml_node entry = xpath_entry.node();
        pugi::xml_node k_ele = entry.child("k_ele");
        pugi::xml_node keb = k_ele.child("keb");

        // std::cout << "oskar: " << keb.text().get() << std::endl;
    }
}

TEST_CASE("learning_libharu") {
    HPDF_Doc pdf = HPDF_New(NULL, NULL);
    REQUIRE(pdf != NULL);
    REQUIRE(HPDF_OK == HPDF_UseUTFEncodings(pdf));
    REQUIRE(HPDF_OK == HPDF_SetCurrentEncoder(pdf, "UTF-8"));

    HPDF_Page page = HPDF_AddPage(pdf);

    HPDF_REAL width = HPDF_Page_GetWidth(page);
    HPDF_REAL height = HPDF_Page_GetHeight(page);

    const char* font_path =
        HPDF_LoadTTFontFromFile(pdf,
                                "noto-sans-jp/NotoSansJP[wght].ttf",
                                HPDF_TRUE
                                );

    REQUIRE(font_path != NULL);
    HPDF_Font font = HPDF_GetFont(pdf, font_path, "UTF-8");
    REQUIRE(font != NULL);
    REQUIRE(HPDF_OK == HPDF_Page_SetFontAndSize(page, font, 24));

    REQUIRE(HPDF_OK == HPDF_Page_BeginText(page));
    REQUIRE(HPDF_OK == HPDF_Page_TextOut(page, 100, height - 100, "åäö世界hejこん"));
    REQUIRE(HPDF_OK == HPDF_Page_EndText(page));

    std::string path = path_for_kanji("04fd7");
    auto svg_files = generate_stroke_order_svg_files(path);
    REQUIRE(svg_files.size() == 9);

    std::string file = svg_files[7];
    auto jpg = svg_to_jpg(file);

    HPDF_Image image = HPDF_LoadJpegImageFromMem(pdf,
                                                 &jpg[0],
                                                 jpg.size()
                                                 );
    
    double iw = HPDF_Image_GetWidth(image);
    double ih = HPDF_Image_GetHeight(image);
    REQUIRE(HPDF_OK == HPDF_Page_DrawImage(page, image, 10, 500, iw, ih));

    REQUIRE(HPDF_OK == HPDF_SaveToFile(pdf, "example.pdf"));

    HPDF_Free(pdf);
}

void iterate_node(pugi::xml_node node) {
    for (pugi::xml_node group_or_stroke : node.children()) {
        std::string id = group_or_stroke.attribute("id").value();

        if (id.find("-g") != std::string::npos) {
            iterate_node(group_or_stroke);
        } else {
            // std::cout << "oskar: " << id << std::endl;
        }
    }
}

TEST_CASE("learning_svg") {
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file("kanjivg/kanji/04fd7.svg");
    REQUIRE(result == true);

    pugi::xml_node svg = doc.child("svg");
    pugi::xml_node stroke_paths = svg.child("g");
    pugi::xml_node root = stroke_paths.child("g");

    iterate_node(root);
}

