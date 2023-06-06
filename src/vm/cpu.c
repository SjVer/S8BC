#include <memory.h>
#include <unistd.h>

#include "instructions.h"
#include "vm/cpu.h"

#define Fetch(cpu) (cpu->memory[cpu->pc++])
#define Fetch_word(cpu) (Fetch(cpu) | Fetch(cpu) << 8)
#define Byte_at_fetch(cpu) (cpu->memory[Fetch_word(cpu)])
#define Word_at_fetch(cpu) (Byte_at_fetch(cpu) | Byte_at_fetch(cpu) << 8)

void log_status(cpu* cpu) {
    int stack_used = STACK_END - (STACK_START | cpu->sp);
    Log(
        "SP: 0x(01)%02x (%d), A: 0x%02x, X: 0x%02x, Y: 0x%02x",
        cpu->sp, stack_used, cpu->a, cpu->x, cpu->y
    );

    byte opcode = cpu->memory[cpu->pc];
    Log(
        "0x%04x: 0x%02x %s (%d %d %d)",
        cpu->pc, opcode, opcode_to_string(opcode),
        opcode >> 7, (opcode >> 4) & 0b111, opcode & 0xf
    );
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

void execute_instr(cpu* cpu) {
    printf("%.*s\n", 10, cpu->memory);

    switch ((opcode)Fetch(cpu)) {
        case OP_NOP:
            break;
        
        // load/store operations
        case OP_LDA_IMM:
            cpu->a = Fetch(cpu);
            break;
        case OP_LDA_ABS:
            cpu->a = Byte_at_fetch(cpu);
            break;
        case OP_LDX_IMM:
            cpu->x = Fetch(cpu);
            break;
        case OP_LDX_ABS:
            cpu->x = Byte_at_fetch(cpu);
            break;
        case OP_LDY_IMM:
            cpu->y = Fetch(cpu);
            break;
        case OP_LDY_ABS:
            cpu->y = Byte_at_fetch(cpu);
            break;
        case OP_STA:
            Byte_at_fetch(cpu) = cpu->a;
            break;
        case OP_STX:
            Byte_at_fetch(cpu) = cpu->x;
            break;
        case OP_STY:
            Byte_at_fetch(cpu) = cpu->y;
            break;
        
        // register operations
        case OP_TAX:
            cpu->x = cpu->a;
            break;
        case OP_TAY:
            cpu->y = cpu->a;
            break;
        case OP_TXA:
            cpu->a = cpu->x;
            break;
        case OP_TYA:
            cpu->a = cpu->y;
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
            break;
        case OP_TXS:
            cpu->sp = cpu->x;
            break;
        case OP_PSH:
            cpu->memory[STACK_START | --cpu->sp] = cpu->a;
            break;
        case OP_PLL:
            cpu->a = cpu->memory[STACK_START | cpu->sp];
            break;
        case OP_POP:
            cpu->a = cpu->memory[STACK_START | cpu->sp++];
            break;

        // bitwise operations
        case OP_AND_IMM:
            cpu->a = cpu->a & Fetch(cpu);
            break;
        case OP_AND_ABS:
            cpu->a = cpu->a & Byte_at_fetch(cpu);
            break;
        case OP_IOR_IMM:
            cpu->a = cpu->a | Fetch(cpu);
            break;
        case OP_IOR_ABS:
            cpu->a = cpu->a | Byte_at_fetch(cpu);
            break;
        case OP_XOR_IMM:
            cpu->a = cpu->a ^ Fetch(cpu);
            break;
        case OP_XOR_ABS:
            cpu->a = cpu->a ^ Byte_at_fetch(cpu);
            break;
        case OP_SHL_IMM:
            cpu->a = cpu->a << Fetch(cpu);
            break;
        case OP_SHL_ABS:
            cpu->a = cpu->a << Byte_at_fetch(cpu);
            break;
        case OP_SHR_IMM:
            cpu->a = cpu->a >> Fetch(cpu);
            break;
        case OP_SHR_ABS:
            cpu->a = cpu->a >> Byte_at_fetch(cpu);
            break;

        // numerical operations
        case OP_ADD_IMM:
            cpu->a = cpu->a + Fetch(cpu);
            break;
        case OP_ADD_ABS:
            cpu->a = cpu->a + Byte_at_fetch(cpu);
            break;
        case OP_SUB_IMM:
            cpu->a = cpu->a - Fetch(cpu);
            break;
        case OP_SUB_ABS:
            cpu->a = cpu->a - Byte_at_fetch(cpu);
            break;
        case OP_MUL_IMM:
            cpu->a = cpu->a * Fetch(cpu);
            break;
        case OP_MUL_ABS:
            cpu->a = cpu->a * Byte_at_fetch(cpu);
            break;
        case OP_DIV_IMM:
            cpu->a = cpu->a / Fetch(cpu);
            break;
        case OP_DIV_ABS:
            cpu->a = cpu->a / Byte_at_fetch(cpu);
            break;
        case OP_INA:
            cpu->a++;
            break;
        case OP_DEA:
            cpu->a--;
            break;
        case OP_INC:
            Byte_at_fetch(cpu)++;
            break;
        case OP_DEC:
            Byte_at_fetch(cpu)--;
            break;

        // control flow operations
        case OP_JMP:
            cpu->pc = Fetch_word(cpu);
            break;
        case OP_JIZ:
            if (cpu->flags.z) cpu->pc = Fetch_word(cpu);
            break;
        case OP_JNZ:
            if (!cpu->flags.z) cpu->pc = Fetch_word(cpu);
            break;
        case OP_JIC:
            if (cpu->flags.c) cpu->pc = Fetch_word(cpu);
            break;
        case OP_JNC:
            if (!cpu->flags.c) cpu->pc = Fetch_word(cpu);
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
}

void execute(cpu* cpu) {
    // separate loops for performance
    if (cli_args.debug) {
        while (!cpu->flags.h) {
            printf("\n");
            log_status(cpu);
            execute_instr(cpu);
            sleep(1);
        }
    } else {
        while (!cpu->flags.h)
            execute_instr(cpu);
    }
}