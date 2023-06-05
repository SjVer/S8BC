#pragma once
#define SPECS_H

#include "common.h"

#define WORD_SIZE 16

typedef uint16_t word;

#define ENDIANNESS "little"

#define MEMORY_SIZE (0xffff + 1)

#define RESET_VECTOR 0xfffe

/*
  memory map:

    0x0000 ┌─────────┐ RAM
           │ zero    │
    0x00ff │ page    │
    0x0100 ├─────────┤
           │ stack   │
    0x01ff │         │
    0x0200 ├─────────┤
           │ i/o     │
    0x02ff │         │
    0x0300 ├─────────┤
           │ unused  │
           │         │
           .         .
           │         │
    0x7fff │         │
    0x8000 ├─────────┤ ROM
           │         │
           .         .
           .         .
           .         .
    0xfffe │(reset   │
    0xffff │ vector) │
           └─────────┘
*/

// 16kb (16384 bytes)
#define RAM_START 0x0000
#define RAM_END 0x3fff

// 255 bytes
#define ZERO_PAGE_START 0x0000
#define ZERO_PAGE_END 0x00ff // incl.

// 255 bytes
#define STACK_START 0x0100
#define STACK_END 0x01ff // incl.

// 255 bytes
#define IO_START 0x0200
#define IO_END 0x02ff // incl.

// 32kb (32768 bytes)
#define ROM_START 0x8000
#define ROM_END 0xffff // incl.

#define ROM_SIZE (ROM_END - ROM_START + 1)
