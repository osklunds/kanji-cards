
#include "create_card.hpp"

#include <cassert>
#include <iostream>
#include <hpdf.h>
#include "stroke_order.hpp"

const HPDF_REAL page_width = 1200;
const HPDF_REAL page_height = 2400;
const HPDF_REAL body_font_size = 40;
const HPDF_REAL body_line_spacing = 50;
const HPDF_REAL left_right_margin = 50;

HPDF_REAL multiline_text_out(HPDF_Page page,
                             std::string text,
                             HPDF_REAL ypos,
                             HPDF_Font font
                             ) {
    HPDF_REAL offset = 0;
    while (!text.empty()) {
        HPDF_UINT num_bytes =
            HPDF_Font_MeasureText(font,
                                  (const HPDF_BYTE*)text.c_str(),
                                  text.size(),
                                  page_width - left_right_margin*2,
                                  body_font_size,
                                  0.0,
                                  0.0,
                                  HPDF_FALSE,
                                  NULL
                                  );

        std::string text_this_iteration {};

        text_this_iteration = text.substr(0, num_bytes);
        text.erase(0, num_bytes);

        assert(HPDF_OK == HPDF_Page_BeginText(page));
        auto res = HPDF_Page_TextOut(page,
                                     left_right_margin,
                                     ypos - body_font_size - offset,
                                     text_this_iteration.c_str()
                                     );
        assert(HPDF_OK == res);
        assert(HPDF_OK == HPDF_Page_EndText(page));

        offset += body_line_spacing;
    }

    return offset;
}

void create_card(const kanji_data& kanji_data,
                 const std::string& dir_path
                 ) {
    // Set up pdf document
    HPDF_Doc pdf = HPDF_New(NULL, NULL);
    assert(pdf);
    assert(HPDF_OK == HPDF_UseUTFEncodings(pdf));
    assert(HPDF_OK == HPDF_SetCurrentEncoder(pdf, "UTF-8"));

    // Set up page
    HPDF_Page page = HPDF_AddPage(pdf);
    assert(page);

    const double page_height = 2400;
    assert(HPDF_OK == HPDF_Page_SetWidth(page, 1200));
    assert(HPDF_OK == HPDF_Page_SetHeight(page, page_height));

    // Set up font
    const char* font_path =
        HPDF_LoadTTFontFromFile(pdf,
                                "../data/NotoSansJP-VariableFont_wght.ttf",
                                HPDF_TRUE
                                );
    assert(font_path);

    HPDF_Font font = HPDF_GetFont(pdf, font_path, "UTF-8");
    assert(font);

    // Main kanji
    const double main_kanji_font_size = 160;
    assert(HPDF_OK == HPDF_Page_SetFontAndSize(page, font, main_kanji_font_size));

    assert(HPDF_OK == HPDF_Page_BeginText(page));
    assert(HPDF_OK == HPDF_Page_TextOut(page,
                                        50,
                                        page_height - 50 - main_kanji_font_size,
                                        kanji_data.get_kanji().c_str()
                                        ));
    assert(HPDF_OK == HPDF_Page_EndText(page));

    // Stroke order
    double offset = 0.0;
    for (auto jpg : kanji_data.get_stroke_order_jpgs()) {
        HPDF_Image image = HPDF_LoadJpegImageFromMem(pdf,
                                                     &jpg[0],
                                                     jpg.size()
                                                     );
    
        double image_width = HPDF_Image_GetWidth(image);
        double image_height = HPDF_Image_GetHeight(image);

        assert(HPDF_OK == HPDF_Page_DrawImage(page,
                                              image,
                                              200 + offset,
                                              page_height - 50 - image_height,
                                              image_width,
                                              image_height
                                              ));

        offset += image_width + 40;
    }

    HPDF_REAL height_offset = 300;

    // Meanings
    assert(HPDF_OK == HPDF_Page_SetFontAndSize(page, font, body_font_size));

    std::string meanings = "Meanings: ";
    for (auto meaning : kanji_data.get_meanings()) {
        meanings += meaning + ", ";
    }
    meanings.pop_back();
    meanings.pop_back();

    height_offset += multiline_text_out(page,
                                        meanings,
                                        page_height - height_offset,
                                        font
                                        );

    // On readings
    assert(HPDF_OK == HPDF_Page_SetFontAndSize(page, font, body_font_size));

    std::string on_readings = "On_readings: ";
    for (auto on_reading : kanji_data.get_on_readings()) {
        on_readings += on_reading + ", ";
    }
    on_readings.pop_back();
    on_readings.pop_back();

    height_offset += multiline_text_out(page,
                                        on_readings,
                                        page_height - height_offset,
                                        font
                                        );

    // Kun readings
    assert(HPDF_OK == HPDF_Page_SetFontAndSize(page, font, body_font_size));

    std::string kun_readings = "Kun_readings: ";
    for (auto kun_reading : kanji_data.get_kun_readings()) {
        kun_readings += kun_reading + ", ";
    }
    kun_readings.pop_back();
    kun_readings.pop_back();

    height_offset += multiline_text_out(page,
                                        kun_readings,
                                        page_height - height_offset,
                                        font
                                        );

    // Words
    for (word_data word_data : kanji_data.get_words()) {
        std::string word_string = word_data.get_word();

        word_string += " ( " + word_data.get_reading() + " ) ";

        for (auto meaning : word_data.get_meanings()) {
            word_string += meaning + ", ";
        }
        word_string.pop_back();
        word_string.pop_back();

        height_offset += multiline_text_out(page,
                                            word_string,
                                            page_height - height_offset,
                                            font
                                            );
        height_offset += 10;
    }

    assert(HPDF_OK == HPDF_SaveToFile(pdf, "example.pdf"));

    HPDF_Free(pdf);
}
