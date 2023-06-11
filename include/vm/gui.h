#pragma once
#define VM_GUI_H

#include "vm/cpu.h"

#define TEXT_SCALE 20
#define WINDOW_WIDTH (TEXT_WIDTH * TEXT_SCALE)
#define WINDOW_HEIGHT (TEXT_HEIGHT * TEXT_SCALE)
#define WINDOW_PADDING 5

#define FONT_PATH "assets/JetBrainsMono-Regular.ttf"
#define FG_COLOR {.r = 255, .g = 255, .b = 255}
#define BG_COLOR {.r = 0, .g = 0, .b = 0}

void init_gui();
void draw_gui(cpu* cpu);
void quit_gui();

