#pragma once

#include <gui/canvas.h>
#include <gui/icon.h>

#include "cards_data.h"
#include "game.h"

typedef struct {
    const LoteriaCard* card;
    uint8_t sung;
    uint8_t total;
    bool paused;
    bool showing_previous;
    bool finished;
    uint8_t auto_mode;
} LoteriaRenderModel;

void loteria_renderer_draw_card(Canvas* canvas, const LoteriaRenderModel* model);
void loteria_renderer_draw_wrapped(
    Canvas* canvas,
    int32_t x,
    int32_t y,
    int32_t width,
    Font font,
    Align horizontal,
    const char* text);
