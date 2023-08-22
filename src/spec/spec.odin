package spec

// Endianness is big!

WORD_SIZE :: 16
Word :: u16

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
          │ unused  │
          │         │
          .         .
          .         .
          │         │
    $7fff │         │
    $8000 ├─────────┤ ROM
          │ general │
          │ rom     │
          .         .
          .         .
          .         .
          │         │
    $fff0 │         │
    $fffc ├─────────┤
          │ vectors │
    $ffff │         │
          └─────────┘
*/

// 32kb
MEMORY_SIZE :: 0xffff + 1

// 16kb (16384 bytes)
RAM_START :: 0x0000
RAM_END :: 0x3fff

// 255 bytes
ZERO_PAGE_START :: 0x0000
ZERO_PAGE_END :: 0x00ff // incl.

// 255 bytes
STACK_START :: 0x0100
STACK_END :: 0x01ff // incl.

// 255 bytes
IO_START :: 0x0200
IO_END :: 0x02ff // incl.

IO_TTY_INPUT  :: 0x0201
IO_TTY_OUTPUT :: 0x0202

// 32kb (32768 bytes)
ROM_START :: 0x8000
ROM_END :: 0xffff // incl.
ROM_SIZE :: (ROM_END - ROM_START + 1)

// 4 bytes (2 vectors)
VECTORS_START :: 0xfffc
VECTORS_END :: 0xffff // incl.

INPUT_VECTOR :: 0xfffc
RESET_VECTOR :: 0xfffe
