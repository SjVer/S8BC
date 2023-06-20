# Control Signals

| bit | name | operation |
| --- | --- | --- |
|  0 | PCE | program counter enable |
|  1 | PCI | program counter in |
|  2 | PCO | program counter out |
|  3 | LAI | low address in |
|  4 | LAO | low address out |
|  5 | HAI | high address in |
|  6 | HAO | high address out |
|  7 | SPI | stack pointer in |
|  8 | SPO | stack pointer out |
| <!-- --> | <!-- --> | <!-- --> |
|  9 | MEI | memory in |
| 10 | MEO | memory out |
| <!-- --> | <!-- --> | <!-- --> |
| 11 | IRI | instruction register in |
| 12 | ARI | A register in |
| 13 | ARO | A register out |
| 14 | XRI | X register in |
| 15 | XRO | X register out |
| 16 | YRI | Y register in |
| 17 | YRO | Y register out |
| <!-- --> | <!-- --> | <!-- --> |
| 18 | SRI | status register in |
| 19 | SRO | status register out |
| 20 | RFI | result flags in |
| 21 | CFI | comparison flags in |
| 22 | IFS | interrupt flag set |
| 23 | IFC | interrupt flag clear |
| 24 | HFS | halt flag set |
| 25 | ZFO | zero flag out |
| 26 | CFO | carry flag out |
| 27 | EFO | equal flag out |
| 28 | LFO | lesser flag out |
| 29 | GFO | greater flag out |
| 30 | IFO | interrupt flag out |
| <!-- --> | <!-- --> | <!-- --> |
| 31 | AAD | ALU add |
| 32 | ASU | ALU subtract |
| 33 | AMU | ALU multiply |
| 34 | ADI | ALU divide 
| 35 | AAN | ALU bitwise and |
| 36 | AIO | ALU bitwise or |
| 37 | AXO | ALU bitwise xor |
| 38 | ASL | ALU shift left |
| 39 | ASR | ALU shift right |
| 40 | ANO | ALU bitwise not |
| 41 | AIN | ALU increment |
| 42 | ADE | ALU decrement |
| 43 | ALO | ALU out |

# Instruction Steps

| step 1 | step 2 |
| --- | --- |
| CO/MI | CE/RD/WR |

| opcode | instruction  | step 3 | step 4 | step 5 | step 4 |
| --- | ---             | ---    | ---    | ---    | ---    |
| $00 | `nop`           |        |        |        |        | 
| $01 | `lda` immediate | CE/
| $02 | `lda` operand X |
| $03 | `lda` operand Y |
| $04 | `lda` absolute  |
| $05 | `ldx` immediate |
| $06 | `ldx` operand Y |
| $07 | `ldx` absolute  |
| $08 | `ldy` immediate |
| $09 | `ldy` operand X |
| $0A | `ldy` absolute  |
| $0B | `ldi` absolute  |
| $0C | `sta` operand X |
| $0D | `sta` operand Y |
| $0E | `sta` absolute  |
| $0F | `stx` absolute  |
| $10 | `sty` absolute  |
| $11 | `sti` absolute  |
| $12 | `tax`           |
| $13 | `tay`           |
| $14 | `txa`           |
| $15 | `tya`           |
| $16 | `sax`           |
| $17 | `say`           |
| $18 | `sxy`           |
| $19 | `tsx`           |
| $1A | `txs`           |
| $1B | `psh` immediate |
| $1C | `psh` implied   |
| $1D | `psh` operand X |
| $1E | `psh` operand Y |
| $1F | `pll`           |
| $20 | `pop` implied   |
| $21 | `pop` operand X |
| $22 | `pop` operand Y |
| $23 | `and` immediate |
| $24 | `and` operand X |
| $25 | `and` absolute  |
| $26 | `ior` immediate |
| $27 | `ior` operand X |
| $28 | `ior` absolute  |
| $29 | `xor` immediate |
| $2A | `xor` operand X |
| $2B | `xor` absolute  |
| $2C | `shl` immediate |
| $2D | `shl` operand X |
| $2E | `shl` absolute  |
| $2F | `shr` immediate |
| $30 | `shr` operand X |
| $31 | `shr` absolute  |
| $32 | `not` implied   |
| $33 | `not` operand X |
| $34 | `not` operand Y |
| $35 | `not` absolute  |
| $36 | `add` immediate |
| $37 | `add` operand X |
| $38 | `add` absolute  |
| $39 | `sub` immediate |
| $3A | `sub` operand X |
| $3B | `sub` absolute  |
| $3C | `mul` immediate |
| $3D | `mul` operand X |
| $3E | `mul` absolute  |
| $3F | `div` immediate |
| $40 | `div` operand X |
| $41 | `div` absolute  |
| $42 | `inc` implied   |
| $43 | `inc` operand X |
| $44 | `inc` operand Y |
| $45 | `inc` absolute  |
| $46 | `dec` implied   |
| $47 | `dec` operand X |
| $48 | `dec` operand Y |
| $49 | `dec` absolute  |
| $4A | `cmp` immediate |
| $4B | `cmp` operand X |
| $4C | `cmp` operand Y |
| $4D | `cmp` absolute  |
| $4E | `jmp`           |
| $4F | `jzs`           |
| $50 | `jzn`           |
| $51 | `jcs`           |
| $52 | `jcn`           |
| $53 | `jes`           |
| $54 | `jen`           |
| $55 | `jls`           |
| $56 | `jln`           |
| $57 | `jgs`           |
| $58 | `jgn`           |
| $59 | `cll`           |
| $5A | `ret`           |
| $5B | `rti`           |
| $5C | `hlt`           |

