
// Copyright (C) 2025 Oskar Lundström

// This file is part of kanji-cards.

// kanji-cards is free software: you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free Software
// Foundation, either version 3 of the License, or (at your option) any later
// version.

// kanji-cards is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
// FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
// details.

// You should have received a copy of the GNU General Public License along with
// kanji-cards. If not, see <https://www.gnu.org/licenses/>.

#include "create_card.hpp"

#include <cassert>
#include <iostream>
#include <hpdf.h>
#include "stroke_order.hpp"

const HPDF_REAL page_width = 1200;
const HPDF_REAL page_height = 2400;
const HPDF_REAL body_font_size = 40;
const HPDF_REAL body_line_spacing = 50;
const HPDF_REAL top_margin = 200;
const HPDF_REAL margin = 50;
const HPDF_REAL line_break_distance = 10;
const HPDF_REAL stroke_order_spacing = 0;
const HPDF_REAL stroke_order_size = 140;
const HPDF_REAL main_kanji_font_size = 160;
const int max_word_meanings = 5;

#define ASSERT_HPDF_OK(function_call)                                       \
    {                                                                       \
        auto R_E_S_U_L_T = function_call;                                   \
        if (R_E_S_U_L_T != HPDF_OK) {                                       \
            std::cout << "libharu call failed: " << std::hex << R_E_S_U_L_T \
                      << std::endl;                                         \
            assert(R_E_S_U_L_T == HPDF_OK);                                 \
        }                                                                   \
    }

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
                                  page_width - margin*2,
                                  body_font_size,
                                  0.0,
                                  0.0,
                                  HPDF_FALSE,
                                  NULL
                                  );

        std::string text_this_iteration {};

        if (num_bytes < text.size()) {
            auto line_break_pos = text.rfind(",", num_bytes);
            if (line_break_pos == std::string::npos) {
                text_this_iteration = text.substr(0, num_bytes);
                text.erase(0, num_bytes);
            } else {
                // If num_bytes == line_break_pos, too much text might be
                // printed. But if that rare case happens, it's not a big deal.
                text_this_iteration = text.substr(0, line_break_pos+1);
                text.erase(0, line_break_pos+1);
            }
        } else {
            text_this_iteration = text;
            text = {};
        }

        // To handle text not being inside/past the bottom margin
        HPDF_REAL translated_y_pos = ypos - body_font_size - offset;
        if (translated_y_pos < margin) {
            break;
        }

        ASSERT_HPDF_OK(HPDF_Page_BeginText(page));
        ASSERT_HPDF_OK(HPDF_Page_TextOut(page,
                                         margin,
                                         translated_y_pos,
                                         text_this_iteration.c_str()
                                         ));
        ASSERT_HPDF_OK(HPDF_Page_EndText(page));

        offset += body_line_spacing;
    }

    return offset;
}

void draw_line(HPDF_Page page,
               HPDF_REAL x_start,
               HPDF_REAL y_start,
               HPDF_REAL x_stop,
               HPDF_REAL y_stop
               ) {
    ASSERT_HPDF_OK(HPDF_Page_MoveTo(page, x_start, y_start));
    ASSERT_HPDF_OK(HPDF_Page_LineTo(page,  x_stop, y_stop));
    ASSERT_HPDF_OK(HPDF_Page_Stroke(page));
}

std::string create_card(const kanji_data& kanji_data,
                        const std::string& dir_path
                        ) {
    // Set up pdf document
    HPDF_Doc pdf = HPDF_New(NULL, NULL);
    assert(pdf);
    ASSERT_HPDF_OK(HPDF_UseUTFEncodings(pdf));
    ASSERT_HPDF_OK(HPDF_SetCurrentEncoder(pdf, "UTF-8"));

    // Set up page
    HPDF_Page page = HPDF_AddPage(pdf);
    assert(page);

    const double page_height = 2400;
    ASSERT_HPDF_OK(HPDF_Page_SetWidth(page, 1200));
    ASSERT_HPDF_OK(HPDF_Page_SetHeight(page, page_height));

    // Set up font
    const char* font_path =
        HPDF_LoadTTFontFromFile(pdf,
                                "noto-sans-jp/NotoSansJP-VariableFont_wght.ttf",
                                HPDF_TRUE
                                );
    assert(font_path);

    HPDF_Font font = HPDF_GetFont(pdf, font_path, "UTF-8");
    assert(font);

    // Main kanji
    ASSERT_HPDF_OK(HPDF_Page_SetFontAndSize(page, font, main_kanji_font_size));

    // top_margin to compensate for iOS app having a banner temporarily
    // covering this area
    const HPDF_REAL main_kanji_y_offset = top_margin + main_kanji_font_size;

    ASSERT_HPDF_OK(HPDF_Page_BeginText(page));
    ASSERT_HPDF_OK(HPDF_Page_TextOut(page,
                                     margin,
                                     page_height - main_kanji_y_offset,
                                     kanji_data.get_kanji().c_str()
                                     ));
    ASSERT_HPDF_OK(HPDF_Page_EndText(page));

    // Frequency
    ASSERT_HPDF_OK(HPDF_Page_SetFontAndSize(page, font, body_font_size));

    ASSERT_HPDF_OK(HPDF_Page_BeginText(page));

    HPDF_REAL frequency_y_offset = main_kanji_y_offset + body_font_size + margin;
    std::string frequency_string = "No. " + std::to_string(kanji_data.get_frequency());
    ASSERT_HPDF_OK(HPDF_Page_TextOut(page,
                                     margin,
                                     page_height - frequency_y_offset,
                                     frequency_string.c_str()
                                     ));
    ASSERT_HPDF_OK(HPDF_Page_EndText(page));

    // Stroke order
    const HPDF_REAL stroke_order_x_start =
        margin + main_kanji_font_size + margin;
    const HPDF_REAL stroke_order_x_end = page_width - margin;
    const HPDF_REAL stroke_order_total_width =
        stroke_order_x_end - stroke_order_x_start;

    HPDF_REAL x_offset = 0.0;
    HPDF_REAL y_offset = top_margin;

    for (auto jpg : kanji_data.get_stroke_order_jpgs()) {
        HPDF_Image image = HPDF_LoadJpegImageFromMem(pdf,
                                                     &jpg[0],
                                                     jpg.size()
                                                     );
    
        HPDF_UINT image_width = HPDF_Image_GetWidth(image);
        HPDF_UINT image_height = HPDF_Image_GetHeight(image);
        assert(image_width == stroke_order_image_size);
        assert(image_height == stroke_order_image_size);

        HPDF_REAL remaining_width = stroke_order_total_width - x_offset;
        if (stroke_order_size + stroke_order_spacing > remaining_width) {
            x_offset = 0.0;
            y_offset += stroke_order_size + stroke_order_spacing;
        }

        HPDF_REAL x_pos = stroke_order_x_start + x_offset;
        HPDF_REAL y_pos = page_height - stroke_order_size - y_offset;

        ASSERT_HPDF_OK(HPDF_Page_DrawImage(page,
                                           image,
                                           x_pos,
                                           y_pos,
                                           stroke_order_size,
                                           stroke_order_size
                                           ));

        ASSERT_HPDF_OK(HPDF_Page_SetRGBStroke(page, 0.7, 0.7, 0.7));
        ASSERT_HPDF_OK(HPDF_Page_Rectangle(page,
                                           x_pos,
                                           y_pos,
                                           stroke_order_size,
                                           stroke_order_size
                                           ));

        ASSERT_HPDF_OK(HPDF_Page_Stroke(page));

        x_offset += stroke_order_size + stroke_order_spacing;
    }

    y_offset += stroke_order_size + margin;

    HPDF_REAL offset_due_to_frequency = frequency_y_offset + margin;
    if (y_offset < offset_due_to_frequency) {
        y_offset = offset_due_to_frequency;
    }

    // Meanings
    draw_line(page,
              margin,
              page_height - y_offset,
              page_width - margin,
              page_height - y_offset
              );
    y_offset += margin;

    ASSERT_HPDF_OK(HPDF_Page_SetFontAndSize(page, font, body_font_size));

    std::string meanings = "Meanings: ";
    for (auto meaning : kanji_data.get_meanings()) {
        meanings += meaning + ", ";
    }
    meanings.pop_back();
    meanings.pop_back();

    y_offset += multiline_text_out(page,
                                   meanings,
                                   page_height - y_offset,
                                   font
                                   );

    // On readings
    y_offset += margin;
    ASSERT_HPDF_OK(HPDF_Page_SetFontAndSize(page, font, body_font_size));

    std::string on_readings = "On readings: ";
    for (auto on_reading : kanji_data.get_on_readings()) {
        on_readings += on_reading + ", ";
    }
    on_readings.pop_back();
    on_readings.pop_back();

    y_offset += multiline_text_out(page,
                                   on_readings,
                                   page_height - y_offset,
                                   font
                                   );

    // Kun readings
    y_offset += margin;
    ASSERT_HPDF_OK(HPDF_Page_SetFontAndSize(page, font, body_font_size));

    std::string kun_readings = "Kun readings: ";
    for (auto kun_reading : kanji_data.get_kun_readings()) {
        kun_readings += kun_reading + ", ";
    }
    kun_readings.pop_back();
    kun_readings.pop_back();

    y_offset += multiline_text_out(page,
                                   kun_readings,
                                   page_height - y_offset,
                                   font
                                   );
    y_offset += margin;

    // Words
    draw_line(page,
              margin,
              page_height - y_offset,
              page_width - margin,
              page_height - y_offset
              );
    y_offset += margin;

    for (word_data word_data : kanji_data.get_words()) {
        std::string word_string = word_data.get_word();

        word_string += " ( " + word_data.get_reading() + " ) ";

        for (size_t i = 0;
             i < word_data.get_meanings().size() && i < max_word_meanings;
             i++
             ) {
            word_string += word_data.get_meanings()[i] + ", ";
        }
        word_string.pop_back();
        word_string.pop_back();

        y_offset += multiline_text_out(page,
                                       word_string,
                                       page_height - y_offset,
                                       font
                                       );
        y_offset += line_break_distance;
    }

    std::string path = std::format("{}/{:0>4}-{}.pdf",
                                   dir_path,
                                   kanji_data.get_frequency(),
                                   kanji_data.get_kanji()
                                   );

    ASSERT_HPDF_OK(HPDF_SaveToFile(pdf, path.c_str()));

    HPDF_Free(pdf);

    return path;
}
