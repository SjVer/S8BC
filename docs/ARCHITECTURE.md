# Architecture

The S8BC (Some 8-Bit CPU) architecture is an 8-Bit little-endian architecture with 8-bit opcodes, 16-bit addressing, 3 registers and a stack.

## Design

The S8BC has 3 general-purpose 8-bit registers: the accumulator, X register and Y register.
It als has an 8-bit status register with the following flags:

- `Z` (bit 1): The result of an operation was 0;
- `C` (bit 2): The result of an operation overflowed;
- `E` (bit 3): A compared value was equal to another;
- `L` (bit 4): A compared value was less than another;
- `G` (bit 5): A compared value was greater than another;
- `I` (bit 7): Interrupts are disabled;
- `H` (bit 8): The CPU is has halted.

Lastly the S8BC has a 16-bit program counter and an 8-bit stack pointer.

## Memory

The S8BC has an addressing range from `$0000` to `$ffff` and has 32kb of RAM and 32kb of ROM.
RAM lives at `$0000-$7fff` and ROM at `$8000-$ffff` and memory is mapped as follows (with inclusive bounds):

- `$0000-$00ff`: Zero page
- `$0100-$01ff`: Stack
- `$0200-$02ff`: I/O
- `$0300-$7fff`: Unused
- `$8000-$fff0`: General ROM
- `$fffc-$ffff`: Interrupt vectors

The I/O section of RAM is used to interface with external hardware. The standard layout is as follows:

- `$0201`: TTY input (written by TTY)
- `$0202`: TTY output (read by TTY)

The interrupt vectors section contains addresses jumped to when interrupts are triggered.
The layout is as follows:

- `$fffc`: Input vector
- `$fffe`: Reset vector

Programs are loaded as ROM at `$8000` and execution starts at the predefined reset vector.

## The Stack 

The stack lives in memory at address `$0100` to `$01ff`.
The address of the top of the stack is composed of the start of the stack (`$0100`) bitwise-OR'ed with the stack pointer.
The stack pointer is initialized as `$ff`, pushing onto the stack decrements it, and popping off the stack increments it.

## Interrupts

The S8BC has builtin interrupts for handling resets and input.
When an interrupt is triggered, the status register and the program counter are pushed onto the stack and a predefined address is jumped to.
These addresses are defined in the interrupt vectors section of ROM.
Interrupts can be disabled by setting the interrupt-disable flag.

## Definitions

The A, X and Y registers are referred to as A, X and Y.
The stack pointer and program counter are referred to as SP and PC.
The flags are referred to as Z, C, E, L, G, I and H are the zero-, carry-, equal-, lesser-, greater-, interrupt-disable-, and halt-flags.

