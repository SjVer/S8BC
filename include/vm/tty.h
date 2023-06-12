#pragma once
#define VM_TTY_H

#include "vm/cpu.h"

#define SCREEN_PATH "/usr/bin/screen"
#define SCREEN_ARGS "-S", APP_NAME, "-t", APP_NAME

void init_tty();
void update_tty(cpu* cpu);
void quit_tty();

