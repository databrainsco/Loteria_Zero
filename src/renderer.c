#include <stdio.h>
#include <furi.h>
#include "renderer.h"

#include <gui/elements.h>
#include <string.h>

static void loteria_draw_text_line(
    Canvas* canvas,
    int32_t x,
    int32_t y,
    Align horizontal,
    const char* text) {
    canvas_draw_str_aligned(canvas, x, y, horizontal, AlignTop, text);
}

void loteria_renderer_draw_wrapped(
    Canvas* canvas,
    int32_t x,
    int32_t y,
    int32_t width,
    Font font,
    Align horizontal,
    const char* text) {
    canvas_set_font(canvas, font);
    FuriString* line = furi_string_alloc();
    size_t len = strlen(text);
    size_t i = 0;
    int32_t cursor_y = y;
    const int32_t line_h = (font == FontPrimary) ? 11 : 9;

    while(i < len) {
        while(i < len && text[i] == ' ') i++;
        if(i >= len) break;

        furi_string_reset(line);
        size_t last_fit = i;
        size_t j = i;
        while(j < len) {
            size_t next = j;
            while(next < len && text[next] != ' ') next++;
            FuriString* trial = furi_string_alloc_set(line);
            if(furi_string_size(trial) > 0) {
                furi_string_push_back(trial, ' ');
            }
            for(size_t k = j; k < next; k++) {
                furi_string_push_back(trial, text[k]);
            }
            if(canvas_string_width(canvas, furi_string_get_cstr(trial)) > (size_t)width) {
                furi_string_free(trial);
                break;
            }
            furi_string_set(line, trial);
            furi_string_free(trial);
            last_fit = next;
            j = next;
            while(j < len && text[j] == ' ') j++;
        }

        if(furi_string_size(line) == 0) {
            /* single long word: hard-cut */
            while(j < len && canvas_string_width(canvas, furi_string_get_cstr(line)) <= (size_t)width) {
                furi_string_push_back(line, text[j]);
                j++;
                last_fit = j;
            }
            if(furi_string_size(line) == 0 && j < len) {
                furi_string_push_back(line, text[j]);
                last_fit = j + 1;
            }
        }

        loteria_draw_text_line(canvas, x, cursor_y, horizontal, furi_string_get_cstr(line));
        cursor_y += line_h;
        i = last_fit;
        if(cursor_y > 120) break;
    }
    furi_string_free(line);
}

void loteria_renderer_draw_card(Canvas* canvas, const LoteriaRenderModel* model) {
    canvas_clear(canvas);
    canvas_set_color(canvas, ColorBlack);

    /* Vertical card layout: logical 64 x 128 */
    canvas_draw_frame(canvas, 0, 0, 64, 128);
    canvas_draw_frame(canvas, 2, 2, 60, 124);

    canvas_set_font(canvas, FontSecondary);
    canvas_draw_str_aligned(canvas, 32, 5, AlignCenter, AlignTop, "LOTERIA ZERO");

    if(model->finished) {
        canvas_set_font(canvas, FontPrimary);
        canvas_draw_str_aligned(canvas, 32, 40, AlignCenter, AlignTop, "FIN DE LA");
        canvas_draw_str_aligned(canvas, 32, 54, AlignCenter, AlignTop, "PARTIDA!");
        canvas_set_font(canvas, FontSecondary);
        loteria_renderer_draw_wrapped(
            canvas,
            32,
            72,
            56,
            FontSecondary,
            AlignCenter,
            "Que no se quede ninguna tabla sin ganar!");
        return;
    }

    if(!model->card) {
        canvas_set_font(canvas, FontSecondary);
        canvas_draw_str_aligned(canvas, 32, 60, AlignCenter, AlignTop, "Sin carta");
        return;
    }

    const Icon* icon = model->card->icon;
    uint16_t iw = icon_get_width(icon);
    uint16_t ih = icon_get_height(icon);
    int32_t ix = (64 - (int32_t)iw) / 2;
    int32_t iy = 16;
    canvas_draw_icon(canvas, ix, iy, icon);

    int32_t text_y = iy + (int32_t)ih + 4;
    canvas_set_font(canvas, FontPrimary);
    canvas_draw_str_aligned(canvas, 32, text_y, AlignCenter, AlignTop, model->card->name);

    text_y += 12;
    loteria_renderer_draw_wrapped(
        canvas, 32, text_y, 56, FontSecondary, AlignCenter, model->card->rhyme);

    char progress[16];
    snprintf(progress, sizeof(progress), "%u / %u", model->sung, model->total);
    canvas_set_font(canvas, FontSecondary);
    canvas_draw_str_aligned(canvas, 32, 116, AlignCenter, AlignTop, progress);

    if(model->showing_previous) {
        canvas_draw_str_aligned(canvas, 32, 108, AlignCenter, AlignTop, "< anterior");
    } else if(model->paused) {
        canvas_draw_str_aligned(canvas, 32, 108, AlignCenter, AlignTop, "PAUSA");
    } else if(model->auto_mode != 0) {
        canvas_draw_str_aligned(canvas, 32, 108, AlignCenter, AlignTop, "AUTO");
    }
}
