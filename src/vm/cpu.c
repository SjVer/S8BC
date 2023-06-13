#include <memory.h>
#define __USE_XOPEN_EXTENDED
#include <unistd.h>

#include "specs.h"
#include "instructions.h"
#include "vm/cpu.h"
#include "vm/tty.h"

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
    cpu->flags.i = false;
}

void load_rom(cpu* cpu, byte* data) {
    memcpy(cpu->memory + ROM_START, data, ROM_SIZE);
}

void load_reset_vector(cpu* cpu) {
    cpu->pc = cpu->memory[RESET_VECTOR]
            | cpu->memory[RESET_VECTOR + 1] << 8;
    
    if (cli_args.verbose)
        Log("loaded reset vector: $%04x", cpu->pc);
}

static void set_flags_by_result(cpu* cpu, unsigned result) {
    cpu->flags.c = result > 0xff;
    cpu->flags.z = result == 0;
}

static void push_to_stack(cpu* cpu, byte value) {
    cpu->memory[STACK_START | --cpu->sp] = value;
}

static byte pop_from_stack(cpu* cpu) {
    return cpu->memory[STACK_START | cpu->sp++];
}

static void push_pc_to_stack(cpu* cpu) {
    push_to_stack(cpu, cpu->pc >> 8);
    push_to_stack(cpu, cpu->pc & 0xff);
}

void start_reset_interrupt(cpu* cpu) {
    if (cpu->flags.i) return;

    reset_cpu(cpu);
    load_reset_vector(cpu);

    if (cli_args.verbose)
        Log("reset interrupt, jumping to $%04x", cpu->pc);
}

void start_input_interrupt(cpu* cpu) {
    if (cpu->flags.i) return;

    if (cli_args.debug || 1) {
        putchar('\n');
        Log("received input: %d", cpu->memory[IO_TTY_INPUT]);
    }

    // TODO: temporary?
    if (cpu->memory[IO_TTY_INPUT] == SIGQUIT) {
        raise(SIGINT);
        return;
    }

    push_pc_to_stack(cpu);
    push_to_stack(cpu, cpu->status);
    
    cpu->pc = cpu->memory[INPUT_VECTOR]
            | cpu->memory[INPUT_VECTOR + 1] << 8;

    if (cli_args.verbose)
        Log("input interrupt, jumping to $%04x", cpu->pc);
}

static void execute_instr(cpu* cpu) {

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
            push_to_stack(cpu, Fetch(cpu));
            break;
        case OP_PSH_IMP:
            push_to_stack(cpu, cpu->a);
            break;
        case OP_PSH_OPX:
            push_to_stack(cpu, cpu->x);
            break;
        case OP_PSH_OPY:
            push_to_stack(cpu, cpu->y);
            break;

        case OP_PLL:
            cpu->a = cpu->memory[STACK_START | cpu->sp];
            break;

        case OP_POP_IMP:
            cpu->a = pop_from_stack(cpu);
            break;
        case OP_POP_OPX:
            cpu->x = pop_from_stack(cpu);
            break;
        case OP_POP_OPY:
            cpu->y = pop_from_stack(cpu);
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

        case OP_NOT_IMP:
            set_flags_by_result(cpu, cpu->a = ~cpu->a);
            break;
        case OP_NOT_OPX:
            set_flags_by_result(cpu, cpu->x = ~cpu->x);
            break;
        case OP_NOT_OPY:
            set_flags_by_result(cpu, cpu->y = ~cpu->y);
            break;
        case OP_NOT_ABS: {
            byte* op = cpu->memory + Fetch_word(cpu);
            set_flags_by_result(cpu, *op = ~(*op));
            break;
        }

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
            push_pc_to_stack(cpu);
            cpu->pc = Fetch_word(cpu);
            break;
        case OP_RET:
            cpu->pc = pop_from_stack(cpu);
            cpu->pc |= pop_from_stack(cpu) << 8;
            break;
        case OP_RTI:
            cpu->status = pop_from_stack(cpu);
            cpu->pc = pop_from_stack(cpu);
            cpu->pc |= pop_from_stack(cpu) << 8;
            break;
        case OP_HLT:
            cpu->flags.h = true;
            break;
    }

#undef Binop_on_a
}

void execute(cpu* cpu) {
    while (!cpu->flags.h) {
        if (cli_args.debug) {
            printf("\n");
            log_status(cpu);
        }
        if (cli_args.slow)
            usleep(1e6 - CYCLE_DELAY);

        execute_instr(cpu);
        update_tty(cpu);

        usleep(CYCLE_DELAY);
    }

    if (cli_args.verbose) Log("execution halted");
}
