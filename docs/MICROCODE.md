# Control Signals

| bit | name | operation | edge |
| --- | --- | --- | --- |
|  | `MEM/DB` | output memory contents to DB
|  | `DB/MEM` | write DB to memory | R
|  | `DB/IR` | write DB to IR | R
| <!-- --> |
|  | `PCI` | PC increment | F
|  | `PCL/DB` | output PC low to DB
|  | `PCH/DB` | output PC high to DB
|  | `ABL/PCL` | write ABL to PC low | R
|  | `ABH/PCH` | write ABH to PC high | R
|  | `PCL/ABL` | output PC low to ABL
|  | `PCH/ABH` | output PC high to ABH
|  | `PC/AB` | output PC to ABL/H
| <!-- --> |
|  | `DB/ARL` | write DB to AR low | R
|  | `DB/ARH` | write DB to AR high | R
|  | `AR/AB` | output AR to ABL/H
|  | `AB/DP` | write ABL/H to DP | R
| <!-- --> |
|  | `DB/SP` | write DB to SP | R
|  | `SP/DB` | output SP to DB
|  | `SPI` | SP increment | F
|  | `SPD` | SP decrement | R
| <!-- --> |
|  | `DB/A` | write DB to A | R
|  | `A/DB` | output A to DB
|  | `ALU/A` | write ALU result to A | R
|  | `A/ALU` | output A to ALU left input
|  | `DB/X` | write DB to X | R
|  | `X/DB` | output X to DB
|  | `DB/Y` | write DB to Y | R
|  | `Y/DB` | output Y to DB
|  | `DB/R` | write DB to R | R
|  | `R/DB` | output R to DB
|  | `ALU/R` | write ALU result to R | R
| <!-- --> |
|  | `END` | reset step counter | F

# Instruction Steps

A step without control signal (`0`) resets the step counter.
The step counter also automatically resets after step 4.

Step 1 is always `PC/AB, AB/DP, MEM/DB, DB/IR; PCI`.

| opcode | instruction  | step | control signals
| --- | ---             | --- | --- |
| $00 | `nop`           | 2 | `0`
| <!-- --> |
| $01 | `lda` immediate | 2 | `PC/AB, AB/DP, MEM/DB, DB/A; PCI`
| $02 | `lda` operand X | 2 | ``
| $03 | `lda` operand Y | 2 | ``
| $04 | `lda` absolute  | 2 | ``
| $05 | `ldx` immediate | 2 | `PC/AB, AB/DP, MEM/DB, DB/X; PCI`
| $06 | `ldx` operand Y | 2 | ``
| $07 | `ldx` absolute  | 2 | ``
| $08 | `ldy` immediate | 2 | `PC/AB, AB/DP, MEM/DB, DB/Y; PCI`
| $09 | `ldy` operand X | 2 | ``
| $0A | `ldy` absolute  | 2 | ``
| $0B | `ldi` absolute  | 2 | ``
| $0C | `sta` operand X | 2 | ``
| $0D | `sta` operand Y | 2 | ``
| $0E | `sta` absolute  | 2 | ``
| $0F | `stx` absolute  | 2 | ``
| $10 | `sty` absolute  | 2 | ``
| $11 | `sti` absolute  | 2 | ``
| <!-- --> |
| $12 | `tax`           | 2 | ``
| $13 | `tay`           | 2 | ``
| $14 | `txa`           | 2 | ``
| $15 | `tya`           | 2 | ``
| $16 | `sax`           | 2 | ``
| $17 | `say`           | 2 | ``
| $18 | `sxy`           | 2 | ``
| $19 | `tsx`           | 2 | ``
| $1A | `txs`           | 2 | ``
| <!-- --> |
| $1B | `psh` immediate | 2 | ``
| $1C | `psh` implied   | 2 | ``
| $1D | `psh` operand X | 2 | ``
| $1E | `psh` operand Y | 2 | ``
| $1F | `pll`           | 2 | ``
| $20 | `pop` implied   | 2 | ``
| $21 | `pop` operand X | 2 | ``
| $22 | `pop` operand Y | 2 | ``
| <!-- --> |
| $23 | `and` immediate | 2 | ``
| $24 | `and` operand X | 2 | ``
| $25 | `and` absolute  | 2 | ``
| $26 | `ior` immediate | 2 | ``
| $27 | `ior` operand X | 2 | ``
| $28 | `ior` absolute  | 2 | ``
| $29 | `xor` immediate | 2 | ``
| $2A | `xor` operand X | 2 | ``
| $2B | `xor` absolute  | 2 | ``
| $2C | `shl` immediate | 2 | ``
| $2D | `shl` operand X | 2 | ``
| $2E | `shl` absolute  | 2 | ``
| $2F | `shr` immediate | 2 | ``
| $30 | `shr` operand X | 2 | ``
| $31 | `shr` absolute  | 2 | ``
| $32 | `not` implied   | 2 | ``
| $33 | `not` operand X | 2 | ``
| $34 | `not` operand Y | 2 | ``
| $35 | `not` absolute  | 2 | ``
| <!-- --> |
| $36 | `add` immediate | 2 | ``
| $37 | `add` operand X | 2 | ``
| $38 | `add` absolute  | 2 | ``
| $39 | `sub` immediate | 2 | ``
| $3A | `sub` operand X | 2 | ``
| $3B | `sub` absolute  | 2 | ``
| $3C | `mul` immediate | 2 | ``
| $3D | `mul` operand X | 2 | ``
| $3E | `mul` absolute  | 2 | ``
| $3F | `div` immediate | 2 | ``
| $40 | `div` operand X | 2 | ``
| $41 | `div` absolute  | 2 | ``
| $42 | `inc` implied   | 2 | ``
| $43 | `inc` operand X | 2 | ``
| $44 | `inc` operand Y | 2 | ``
| $45 | `inc` absolute  | 2 | ``
| $46 | `dec` implied   | 2 | ``
| $47 | `dec` operand X | 2 | ``
| $48 | `dec` operand Y | 2 | ``
| $49 | `dec` absolute  | 2 | ``
| $4A | `cmp` immediate | 2 | ``
| $4B | `cmp` operand X | 2 | ``
| $4C | `cmp` operand Y | 2 | ``
| $4D | `cmp` absolute  | 2 | ``
| <!-- --> |
| $4E | `jmp`           | 2 | ``
| $4F | `jzs`           | 2 | ``
| $50 | `jzn`           | 2 | ``
| $51 | `jcs`           | 2 | ``
| $52 | `jcn`           | 2 | ``
| $53 | `jes`           | 2 | ``
| $54 | `jen`           | 2 | ``
| $55 | `jls`           | 2 | ``
| $56 | `jln`           | 2 | ``
| $57 | `jgs`           | 2 | ``
| $58 | `jgn`           | 2 | ``
| $59 | `cll`           | 2 | ``
| $5A | `ret`           | 2 | ``
| $5B | `rti`           | 2 | ``
| $5C | `hlt`           | 2 | ``

