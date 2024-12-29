
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

    assert(HPDF_OK == HPDF_Page_SetWidth(page, 1200));
    assert(HPDF_OK == HPDF_Page_SetHeight(page, 2400));

    // Set up font
    const char* font_path =
        HPDF_LoadTTFontFromFile(pdf,
                                "../data/NotoSansJP-VariableFont_wght.ttf",
                                HPDF_TRUE
                                );
    assert(font_path);

    HPDF_Font font = HPDF_GetFont(pdf, font_path, "UTF-8");
    assert(font);
    assert(HPDF_OK == HPDF_Page_SetFontAndSize(page, font, 24));

    assert(HPDF_OK == HPDF_Page_BeginText(page));
    assert(HPDF_OK == HPDF_Page_TextOut(page, 100, 100, "åäö世界hejこん"));
    assert(HPDF_OK == HPDF_Page_EndText(page));

    std::string path = path_for_kanji("04fd7");
    auto svg_files = generate_stroke_order_svg_files(path);
    assert(svg_files.size() == 9);

    std::string file = svg_files[7];
    auto jpg = svg_to_jpg(file);

    HPDF_Image image = HPDF_LoadJpegImageFromMem(pdf,
                                                 &jpg[0],
                                                 jpg.size()
                                                 );
    
    double iw = HPDF_Image_GetWidth(image);
    double ih = HPDF_Image_GetHeight(image);
    assert(HPDF_OK == HPDF_Page_DrawImage(page, image, 10, 500, iw, ih));

    assert(HPDF_OK == HPDF_SaveToFile(pdf, "example.pdf"));

    HPDF_Free(pdf);
}
