#pragma once
#define VM_CPU_H

#include "vm/common.h"
#include "specs.h"

typedef struct cpu {
    byte memory[MEMORY_SIZE];

    byte a, x, y;
    byte sp;
    word pc;

    struct {
        bool z : 1; // zero
        bool c : 1; // carry
        bool h : 1; // halt
        int _  : 6;
    } flags;
} cpu;

void log_status(cpu* cpu);

void reset_cpu(cpu* cpu);
void load_rom(cpu* cpu, byte* data, word size);
void load_reset_vector(cpu* cpu);

void execute_instr(cpu* cpu);
void execute(cpu* cpu);
