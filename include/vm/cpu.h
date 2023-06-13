#pragma once
#define VM_CPU_H

#include "vm/common.h"
#include "specs.h"

typedef struct cpu {
    byte memory[MEMORY_SIZE];

    byte a, x, y;
    byte sp;
    word pc;

    union {
        struct {
            bool z : 1; // zero
            bool c : 1; // carry
            bool e : 1; // equal
            bool l : 1; // less than
            bool g : 1; // greater than
            bool _ : 1;
            bool i : 1; // disable interrupts
            bool h : 1; // halt
        } flags;
        byte status;
    };
} cpu;

void log_status(cpu* cpu);

void reset_cpu(cpu* cpu);
void load_rom(cpu* cpu, byte* data);
void load_reset_vector(cpu* cpu);

void start_reset_interrupt(cpu* cpu);
void start_input_interrupt(cpu* cpu);

void execute(cpu* cpu);
