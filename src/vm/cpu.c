#include <memory.h>
#define __USE_XOPEN_EXTENDED
#include <unistd.h>

#include "instructions.h"
#include "vm/cpu.h"

#define Addr_x(cpu) (((cpu)->pc & 0xff00) | (cpu)->x)
#define Addr_y(cpu) (((cpu)->pc & 0xff00) | (cpu)->y)

#define Fetch(cpu) (cpu->memory[cpu->pc++])
#define Fetch_word(cpu) (Fetch(cpu) | Fetch(cpu) << 8)
#define Byte_at_fetch(cpu) (cpu->memory[Fetch_word(cpu)])

void log_status(cpu* cpu) {
    int stack_used = STACK_END - (STACK_START | cpu->sp);
    Log(
        "PC: $%04x, SP: $%02x (%d)",
        cpu->pc, cpu->sp, stack_used
    );

    Log(
        "A: $%02x, X: $%02x, Y: $%02x",
        cpu->a, cpu->x, cpu->y
    );
    Log(
        "Z: %d, C: %d, H: %d",
        cpu->flags.z, cpu->flags.c, cpu->flags.h
    );

    byte opcode = cpu->memory[cpu->pc];
    Log(
        "$%04x: $%02x %s",
        cpu->pc, opcode, opcode_to_string(opcode)
    );
}

void display_text_output(cpu* cpu) {
    printf("\e[1;1H");

    printf("┌");
    for (int i = 0; i < TEXT_WIDTH; i++) printf("─");
    printf("┐\n");

    for (int y = 0; y < TEXT_HEIGHT; y++) {
        int offset = TEXT_START + y * TEXT_WIDTH;
        printf("│%-*.*s│\n", TEXT_WIDTH, TEXT_WIDTH, cpu->memory + offset);
    }

    printf("└");
    for (int i = 0; i < TEXT_WIDTH; i++) printf("─");
    printf("┘\n");
}

void reset_cpu(cpu* cpu) {
    memset(cpu->memory, 0, MEMORY_SIZE);
    cpu->a = 0;
    cpu->x = 0;
    cpu->y = 0;
    cpu->sp = STACK_END & 0xff;
    cpu->pc = 0;
    cpu->flags.z = false;
    cpu->flags.c = false;
    cpu->flags.h = false;
}

void load_rom(cpu* cpu, byte* data) {
    memcpy(cpu->memory + ROM_START, data, ROM_SIZE);
}

void load_reset_vector(cpu* cpu) {
    cpu->pc = cpu->memory[RESET_VECTOR]
            | cpu->memory[RESET_VECTOR + 1] << 8;
    
    if (cli_args.debug) {
        Log("loaded reset vector: $%04x", cpu->pc);
        Log("first opcode: $%02x", cpu->memory[cpu->pc]);
    }
}

void set_flags_by_result(cpu* cpu, unsigned result) {
    cpu->flags.c = result > 0xff;
    cpu->flags.z = result == 0;
}

void execute_instr(cpu* cpu) {

#define Binop_on_a(op, rhs) { \
        unsigned result = cpu->a op rhs; \
        set_flags_by_result(cpu, result); \
        cpu->a = result; }

    switch ((opcode)Fetch(cpu)) {
        case OP_NOP:
            break;
        
        // load/store operations

        case OP_LDA_IMM:
            cpu->a = Fetch(cpu);
            set_flags_by_result(cpu, cpu->a);
            break;
        case OP_LDA_OPX:
            cpu->a = cpu->memory[Addr_x(cpu)];
            set_flags_by_result(cpu, cpu->a);
            break;
        case OP_LDA_OPY:
            cpu->a = cpu->memory[Addr_y(cpu)];
            set_flags_by_result(cpu, cpu->a);
            break;
        case OP_LDA_ABS:
            cpu->a = Byte_at_fetch(cpu);
            set_flags_by_result(cpu, cpu->a);
            break;

        case OP_LDX_IMM:
            cpu->x = Fetch(cpu);
            set_flags_by_result(cpu, cpu->x);
            break;
        case OP_LDX_OPY:
            cpu->x = cpu->memory[Addr_y(cpu)];
            set_flags_by_result(cpu, cpu->x);
            break;
        case OP_LDX_ABS:
            cpu->x = Byte_at_fetch(cpu);
            set_flags_by_result(cpu, cpu->x);
            break;

        case OP_LDY_IMM:
            cpu->y = Fetch(cpu);
            set_flags_by_result(cpu, cpu->y);
            break;
        case OP_LDY_OPX:
            cpu->y = cpu->memory[Addr_x(cpu)];
            set_flags_by_result(cpu, cpu->y);
            break;
        case OP_LDY_ABS:
            cpu->y = Byte_at_fetch(cpu);
            set_flags_by_result(cpu, cpu->y);
            break;

        case OP_LDI_ABS: {
            word op = Fetch_word(cpu);
            word addr = cpu->memory[op] | cpu->memory[op + 1] << 8;
            cpu->a = cpu->memory[addr];
            set_flags_by_result(cpu, cpu->a);
            break;
        }

        case OP_STA_OPX:
            cpu->memory[Addr_x(cpu)] = cpu->a;
            break;
        case OP_STA_OPY:
            cpu->memory[Addr_y(cpu)] = cpu->a;
            break;
        case OP_STA_ABS:
            Byte_at_fetch(cpu) = cpu->a;
            break;
            
        case OP_STX_ABS:
            Byte_at_fetch(cpu) = cpu->x;
            break;
        case OP_STY_ABS:
            Byte_at_fetch(cpu) = cpu->y;
            break;
        case OP_STI_ABS: {
            word op = Fetch_word(cpu);
            word addr = cpu->memory[op] | cpu->memory[op + 1] << 8;
            cpu->memory[addr] = cpu->a;
            break;
        }
        
        // register operations

        case OP_TAX:
            cpu->x = cpu->a;
            set_flags_by_result(cpu, cpu->x);
            break;
        case OP_TAY:
            cpu->y = cpu->a;
            set_flags_by_result(cpu, cpu->y);
            break;
        case OP_TXA:
            cpu->a = cpu->x;
            set_flags_by_result(cpu, cpu->a);
            break;
        case OP_TYA:
            cpu->a = cpu->y;
            set_flags_by_result(cpu, cpu->a);
            break;
        case OP_SAX: {
            byte tmp = cpu->x;
            cpu->x = cpu->a;
            cpu->a = tmp;
            break;
        }
        case OP_SAY: {
            byte tmp = cpu->y;
            cpu->y = cpu->a;
            cpu->a = tmp;
            break;
        }
        case OP_SXY: {
            byte tmp = cpu->y;
            cpu->y = cpu->x;
            cpu->x = tmp;
            break;
        }

        // stack operations

        case OP_TSX:
            cpu->x = cpu->sp;
            set_flags_by_result(cpu, cpu->x);
            break;
        case OP_TXS:
            cpu->sp = cpu->x;
            break;

        case OP_PSH_IMM:
            cpu->memory[STACK_START | --cpu->sp] = Fetch(cpu);
            break;
        case OP_PSH_IMP:
            cpu->memory[STACK_START | --cpu->sp] = cpu->a;
            break;
        case OP_PSH_OPX:
            cpu->memory[STACK_START | --cpu->sp] = cpu->x;
            break;
        case OP_PSH_OPY:
            cpu->memory[STACK_START | --cpu->sp] = cpu->y;
            break;

        case OP_PLL:
            cpu->a = cpu->memory[STACK_START | cpu->sp];
            break;
        case OP_POP:
            cpu->a = cpu->memory[STACK_START | cpu->sp++];
            break;

        // bitwise operations

        case OP_AND_IMM:
            Binop_on_a(&, Fetch(cpu));
            break;
        case OP_AND_OPX:
            Binop_on_a(&, cpu->x);
            break;
        case OP_AND_ABS:
            Binop_on_a(&, Byte_at_fetch(cpu));
            break;
            
        case OP_IOR_IMM:
            Binop_on_a(|, Fetch(cpu));
            break;
        case OP_IOR_OPX:
            Binop_on_a(|, cpu->x);
            break;
        case OP_IOR_ABS:
            Binop_on_a(|, Byte_at_fetch(cpu));
            break;
            
        case OP_XOR_IMM:
            Binop_on_a(^, Fetch(cpu));
            break;
        case OP_XOR_OPX:
            Binop_on_a(^, cpu->x);
            break;
        case OP_XOR_ABS:
            Binop_on_a(^, Byte_at_fetch(cpu));
            break;
            
        case OP_SHL_IMM:
            Binop_on_a(<<, Fetch(cpu));
            break;
        case OP_SHL_OPX:
            Binop_on_a(<<, cpu->x);
            break;
        case OP_SHL_ABS:
            Binop_on_a(<<, Byte_at_fetch(cpu));
            break;
            
        case OP_SHR_IMM:
            Binop_on_a(>>, Fetch(cpu));
            break;
        case OP_SHR_OPX:
            Binop_on_a(>>, cpu->x);
            break;
        case OP_SHR_ABS:
            Binop_on_a(>>, Byte_at_fetch(cpu));
            break;

        // numerical operations

        case OP_ADD_IMM:
            Binop_on_a(+, Fetch(cpu));
            break;
        case OP_ADD_OPX:
            Binop_on_a(+, cpu->x);
            break;
        case OP_ADD_ABS:
            Binop_on_a(+, Byte_at_fetch(cpu));
            break;

        case OP_SUB_IMM:
            Binop_on_a(-, Fetch(cpu));
            break;
        case OP_SUB_OPX:
            Binop_on_a(-, cpu->x);
            break;
        case OP_SUB_ABS:
            Binop_on_a(-, Byte_at_fetch(cpu));
            break;

        case OP_MUL_IMM:
            Binop_on_a(*, Fetch(cpu));
            break;
        case OP_MUL_OPX:
            Binop_on_a(*, cpu->x);
            break;
        case OP_MUL_ABS:
            Binop_on_a(*, Byte_at_fetch(cpu));
            break;

        case OP_DIV_IMM:
            Binop_on_a(/, Fetch(cpu));
            break;
        case OP_DIV_OPX:
            Binop_on_a(/, cpu->x);
            break;
        case OP_DIV_ABS:
            Binop_on_a(/, Byte_at_fetch(cpu));
            break;

        case OP_INC_IMP:
            set_flags_by_result(cpu, ++cpu->a);
            break;
        case OP_INC_OPX:
            set_flags_by_result(cpu, ++cpu->x);
            break;
        case OP_INC_OPY:
            set_flags_by_result(cpu, ++cpu->y);
            break;
        case OP_INC_ABS:
            set_flags_by_result(cpu, ++Byte_at_fetch(cpu));
            break;

        case OP_DEC_IMP:
            set_flags_by_result(cpu, --cpu->a);
            break;
        case OP_DEC_OPX:
            set_flags_by_result(cpu, --cpu->x);
            break;
        case OP_DEC_OPY:
            set_flags_by_result(cpu, --cpu->y);
            break;
        case OP_DEC_ABS:
            set_flags_by_result(cpu, --Byte_at_fetch(cpu));
            break;

        // control flow operations

        case OP_JMP:
            cpu->pc = Fetch_word(cpu);
            break;
        case OP_JIZ:
            if (cpu->flags.z) cpu->pc = Fetch_word(cpu);
            else cpu->pc += 2;
            break;
        case OP_JNZ:
            if (!cpu->flags.z) cpu->pc = Fetch_word(cpu);
            else cpu->pc += 2;
            break;
        case OP_JIC:
            if (cpu->flags.c) cpu->pc = Fetch_word(cpu);
            else cpu->pc += 2;
            break;
        case OP_JNC:
            if (!cpu->flags.c) cpu->pc = Fetch_word(cpu);
            else cpu->pc += 2;
            break;
        case OP_CLL:
            cpu->pc = Fetch_word(cpu);
            cpu->memory[STACK_START | --cpu->sp] = cpu->pc >> 8;
            cpu->memory[STACK_START | --cpu->sp] = cpu->pc & 0xff;
            break;
        case OP_RET:
            cpu->pc = cpu->memory[STACK_START | cpu->sp++];
            cpu->pc |= cpu->memory[STACK_START | cpu->sp++] << 8;
            break;
        case OP_HLT:
            cpu->flags.h = true;
            break;
    }

#undef Binop_on_a
}

void execute(cpu* cpu) {
    printf("\e[2J");
    
    while (!cpu->flags.h) {
        if (cli_args.debug) {
            printf("\n");
            log_status(cpu);
            usleep(1e6 - CYCLE_DELAY);
        }

        execute_instr(cpu);
        display_text_output(cpu);

        usleep(CYCLE_DELAY);
    }
}