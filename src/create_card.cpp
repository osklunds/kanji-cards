
#include "create_card.hpp"

#include <cassert>
#include <hpdf.h>
#include "stroke_order.hpp"

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

    const double main_kanji_font_size = 160;
    assert(HPDF_OK == HPDF_Page_SetFontAndSize(page, font, main_kanji_font_size));

    assert(HPDF_OK == HPDF_Page_BeginText(page));
    assert(HPDF_OK == HPDF_Page_TextOut(page,
                                        50,
                                        page_height - 50 - main_kanji_font_size,
                                        kanji_data.get_kanji().c_str()
                                        ));
    assert(HPDF_OK == HPDF_Page_EndText(page));

    assert(HPDF_OK == HPDF_SaveToFile(pdf, "example.pdf"));

    HPDF_Free(pdf);
}
