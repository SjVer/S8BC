# Control Signals

| bit | name | operation | edge |
| --- | --- | --- | --- |
|  |  | 
| <!-- --> |
|  | PCI | PC increment | F
|  | PCL/DB | output PC low to DB
|  | PCH/DB | output PC high to DB
|  | ABL/PCL | write ABL to PC low | R
|  | ABH/PCH | write ABH to PC high | R
|  | PCL/ABL | output PC low to ABL
|  | PCH/ABH | output PC high to ABH
| <!-- --> |
|  | DB/ARL | write DB to AR low | R
|  | DB/ARH | write DB to AR high | R
|  | ARL/ABL | output AR low to ABL
|  | ARH/ABH | output AR high to ABH
|  | ABL/DPL | write ABL to DP low | R
|  | ABH/DPH | write ABH to DP high | R
| <!-- --> |
|  | DB/SP | write DB to SP | R
|  | SP/DB | output SP to DB
|  | SPI | SP increment | F
|  | SPD | SP decrement | R
| <!-- --> |
|  | DB/A | write DB to A | R
|  | A/DB | output A to DB
|  | ALU/A | write ALU result to A | R
|  | A/ALU | output A to ALU left input
|  | DB/X | write DB to X | R
|  | X/DB | output X to DB
|  | DB/Y | write DB to Y | R
|  | Y/DB | output Y to DB
|  | DB/R | write DB to R | R
|  | R/DB | output R to DB
|  | ALU/R | write ALU result to R | R
|  |  | 

# Instruction Steps

Step 1 is always ``.

| opcode | instruction  | step | control signals
| --- | ---             | --- | --- |
| $00 | `nop`           |
| <!-- --> |
| $01 | `lda` immediate |
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
| <!-- --> |
| $12 | `tax`           |
| $13 | `tay`           |
| $14 | `txa`           |
| $15 | `tya`           |
| $16 | `sax`           |
| $17 | `say`           |
| $18 | `sxy`           |
| $19 | `tsx`           |
| $1A | `txs`           |
| <!-- --> |
| $1B | `psh` immediate |
| $1C | `psh` implied   |
| $1D | `psh` operand X |
| $1E | `psh` operand Y |
| $1F | `pll`           |
| $20 | `pop` implied   |
| $21 | `pop` operand X |
| $22 | `pop` operand Y |
| <!-- --> |
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
| <!-- --> |
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
| <!-- --> |
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

