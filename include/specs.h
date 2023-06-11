#pragma once
#define SPECS_H

#include "common.h"

#define WORD_SIZE 16

typedef uint16_t word;

#define ENDIANNESS "little"

#define CYCLE_DELAY 2500

#define MEMORY_SIZE (0xffff + 1)

#define RESET_VECTOR 0xfffe

/*
  memory map:

    $0000 ┌─────────┐ RAM
          │ zero    │
    $00ff │ page    │
    $0100 ├─────────┤
          │ stack   │
    $01ff │         │
    $0200 ├─────────┤
          │ i/o     │
    $02ff │         │
    $0300 ├─────────┤
          │ text    │
          │ output  │
    $04ff │         │
    $0500 ├─────────┤
          │ unused  │
          │         │
          .         .
          .         .
          │         │
    $7fff │         │
    $8000 ├─────────┤ ROM
          │         │
          .         .
          .         .
          .         .
    $fffe │(reset   │
    $ffff │ vector) │
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

// 1024 bytes
#define TEXT_START 0x0300
#define TEXT_END 0x04ff // incl.
#define TEXT_WIDTH 32
#define TEXT_HEIGHT 16

// 32kb (32768 bytes)
#define ROM_START 0x8000
#define ROM_END 0xffff // incl.

#define ROM_SIZE (ROM_END - ROM_START + 1)
