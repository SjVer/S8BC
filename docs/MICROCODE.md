# Control Signals

| bit | name | operation | edge |
| --- | --- | --- | --- |
|  | `DB/ABL` | connect DB and ABL
| <!-- --> |
|  | `MEM/DB` | output memory contents to DB
|  | `DB/MEM` | write DB to memory | R
|  | `AB/DP` | write ABL/H to DP | R
| <!-- --> |
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
| <!-- --> |
|  | `DB/SP` | write DB to SP | R
|  | `SP/DB` | output SP to DB
|  | `SP/AB` | output SP and $01 to ABL/H
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
|  | `DB/S` | write DB to S | R
|  | `S/DB` | output S to DB
|  | `SRF` | set result flags | R
|  | `SCF` | set comparison flags | R
|  | `SIF` | set I flag | R
|  | `CIF` | clear I flag | R
|  | `SHF` | set H flag | R
|  | `Z/JE` | output Z to jump-enable
|  | `C/JE` | output C to jump-enable
|  | `E/JE` | output E to jump-enable
|  | `L/JE` | output L to jump-enable
|  | `G/JE` | output G to jump-enable
|  | `I/JE` | output I to jump-enable
|  | `IJE` | invert jump-enable
| <!-- --> |
|  | `END` | reset step counter | F

# Instruction Steps

Step 1 is always `PC/AB, AB/DP, MEM/DB, DB/IR; PCI`.

| opcode | instruction  | step | control signals
| --- | ---             | --- | --- |
| $00 | `nop`           | 2 | `0`
| <!-- --> |
| $01 | `lda` immediate | 2 | `PC/AB, AB/DP, MEM/DB, DB/A; PCI`
| $02 | `lda` operand X | 2 | `X/DB, DB/ABL, PCH/ABH, AB/DP`
|     |                 | 3 | `MEM/DB, DB/A`
| $03 | `lda` operand Y | 2 | `Y/DB, DB/ABL, PCH/ABH, AB/DP`
|     |                 | 3 | `MEM/DB, DB/A`
| $04 | `lda` absolute  | 2 | `PC/AB, AB/DP, MEM/DB, DB/ARL; PCI`
|     |                 | 3 | `PC/AB, AB/DP, MEM/DB, DB/ARH; PCI`
|     |                 | 4 | `AR/AB, AB/DP, MEM/DB, DB/A`
| $05 | `ldx` immediate | 2 | `PC/AB, AB/DP, MEM/DB, DB/X; PCI`
| $06 | `ldx` operand Y | 2 | `Y/DB, DB/ABL, PCH/ABH, AB/DP`
|     |                 | 3 | `MEM/DB, DB/X`
| $07 | `ldx` absolute  | 2 | `PC/AB, AB/DP, MEM/DB, DB/ARL; PCI`
|     |                 | 3 | `PC/AB, AB/DP, MEM/DB, DB/ARH; PCI`
|     |                 | 4 | `AR/AB, AB/DP, MEM/DB, DB/X`
| $08 | `ldy` immediate | 2 | `PC/AB, AB/DP, MEM/DB, DB/Y; PCI`
| $09 | `ldy` operand X | 2 | `X/DB, DB/ABL, PCH/ABH, AB/DP`
|     |                 | 3 | `MEM/DB, DB/Y`
| $0A | `ldy` absolute  | 2 | `PC/AB, AB/DP, MEM/DB, DB/ARL; PCI`
|     |                 | 3 | `PC/AB, AB/DP, MEM/DB, DB/ARH; PCI`
|     |                 | 4 | `AR/AB, AB/DP, MEM/DB, DB/Y`
| $0B | `ldi` absolute  | 2 | ``
| $0C | `sta` operand X | 2 | `X/DB, DB/ABL, PCH/ABH, AB/DP`
|     |                 | 3 | `A/DB, DB/MEM`
| $0D | `sta` operand Y | 2 | `Y/DB, DB/ABL, PCH/ABH, AB/DP`
|     |                 | 3 | `A/DB, DB/MEM`
| $0E | `sta` absolute  | 2 | `PC/AB, AB/DP, MEM/DB, DB/ARL; PCI`
|     |                 | 3 | `PC/AB, AB/DP, MEM/DB, DB/ARH; PCI`
|     |                 | 4 | `AR/AB, AB/DP, A/DB, DB/MEM`
| $0F | `stx` absolute  | 2 | `PC/AB, AB/DP, MEM/DB, DB/ARL; PCI`
|     |                 | 3 | `PC/AB, AB/DP, MEM/DB, DB/ARH; PCI`
|     |                 | 4 | `AR/AB, AB/DP, X/DB, DB/MEM`
| $10 | `sty` absolute  | 2 | `PC/AB, AB/DP, MEM/DB, DB/ARL; PCI`
|     |                 | 3 | `PC/AB, AB/DP, MEM/DB, DB/ARH; PCI`
|     |                 | 4 | `AR/AB, AB/DP, Y/DB, DB/MEM`
| $11 | `sti` absolute  | 2 | ``
| <!-- --> |
| $12 | `tax`           | 2 | `A/DB, DB/X`
| $13 | `tay`           | 2 | `A/DB, DB/Y`
| $14 | `txa`           | 2 | `X/DB, DB/A`
| $15 | `tya`           | 2 | `Y/DB, DB/A`
| $16 | `sax`           | 2 | `A/DB, DB/R`
|     |                 | 3 | `X/DB, DB/A`
|     |                 | 4 | `R/DB, DB/X`
| $17 | `say`           | 2 | `A/DB, DB/R`
|     |                 | 3 | `Y/DB, DB/A`
|     |                 | 4 | `R/DB, DB/Y`
| $18 | `sxy`           | 2 | `X/DB, DB/R`
|     |                 | 3 | `Y/DB, DB/X`
|     |                 | 4 | `R/DB, DB/Y`
| $19 | `tsx`           | 2 | `SP/DB, DB/X`
| $1A | `txs`           | 2 | `X/DB, DB/SP`
| <!-- --> |
| $1B | `psh` immediate | 2 | ``
| $1C | `psh` implied   | 2 | `SPD`
|     |                 | 3 | `SP/AB, AB/DP, A/DB, DB/MEM`
| $1D | `psh` operand X | 2 | `SPD`
|     |                 | 3 | `SP/AB, AB/DP, X/DB, DB/MEM`
| $1E | `psh` operand Y | 2 | `SPD`
|     |                 | 3 | `SP/AB, AB/DP, Y/DB, DB/MEM`
| $1F | `pll`           | 2 | `SP/AB, AB/DP, MEM/DB, DB/A`
| $20 | `pop` implied   | 2 | `SP/AB, AB/DP, MEM/DB, DB/A; SPI`
| $21 | `pop` operand X | 2 | `SP/AB, AB/DP, MEM/DB, DB/X; SPI`
| $22 | `pop` operand Y | 2 | `SP/AB, AB/DP, MEM/DB, DB/Y; SPI`
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
| $4E | `jmp`           | 2 | `PC/AB, AB/DP, MEM/DB, DB/ARL; PCI`
|     |                 | 3 | `PC/AB, AB/DP, MEM/DB, DB/ARH; PCI`
|     |                 | 4 | `AR/AB, ABL/PCL, ABH/PCH`
| $4F | `jzs`           | 2 | `PC/AB, AB/DP, MEM/DB, DB/ARL; PCI`
|     |                 | 3 | `PC/AB, AB/DP, MEM/DB, DB/ARH; PCI`
|     |                 | 4 | `AR/AB, ABL/PCL, ABH/PCH, Z/JE`
| $50 | `jzn`           | 2 | `PC/AB, AB/DP, MEM/DB, DB/ARL; PCI`
|     |                 | 3 | `PC/AB, AB/DP, MEM/DB, DB/ARH; PCI`
|     |                 | 4 | `AR/AB, ABL/PCL, ABH/PCH, Z/JE, IJE`
| $51 | `jcs`           | 2 | `PC/AB, AB/DP, MEM/DB, DB/ARL; PCI`
|     |                 | 3 | `PC/AB, AB/DP, MEM/DB, DB/ARH; PCI`
|     |                 | 4 | `AR/AB, ABL/PCL, ABH/PCH, C/JE`
| $52 | `jcn`           | 2 | `PC/AB, AB/DP, MEM/DB, DB/ARL; PCI`
|     |                 | 3 | `PC/AB, AB/DP, MEM/DB, DB/ARH; PCI`
|     |                 | 4 | `AR/AB, ABL/PCL, ABH/PCH, C/JE, IJE`
| $53 | `jes`           | 2 | `PC/AB, AB/DP, MEM/DB, DB/ARL; PCI`
|     |                 | 3 | `PC/AB, AB/DP, MEM/DB, DB/ARH; PCI`
|     |                 | 4 | `AR/AB, ABL/PCL, ABH/PCH, E/JE`
| $54 | `jen`           | 2 | `PC/AB, AB/DP, MEM/DB, DB/ARL; PCI`
|     |                 | 3 | `PC/AB, AB/DP, MEM/DB, DB/ARH; PCI`
|     |                 | 4 | `AR/AB, ABL/PCL, ABH/PCH, E/JE, IJE`
| $55 | `jls`           | 2 | `PC/AB, AB/DP, MEM/DB, DB/ARL; PCI`
|     |                 | 3 | `PC/AB, AB/DP, MEM/DB, DB/ARH; PCI`
|     |                 | 4 | `AR/AB, ABL/PCL, ABH/PCH, L/JE`
| $56 | `jln`           | 2 | `PC/AB, AB/DP, MEM/DB, DB/ARL; PCI`
|     |                 | 3 | `PC/AB, AB/DP, MEM/DB, DB/ARH; PCI`
|     |                 | 4 | `AR/AB, ABL/PCL, ABH/PCH, L/JE, IJE`
| $57 | `jgs`           | 2 | `PC/AB, AB/DP, MEM/DB, DB/ARL; PCI`
|     |                 | 3 | `PC/AB, AB/DP, MEM/DB, DB/ARH; PCI`
|     |                 | 4 | `AR/AB, ABL/PCL, ABH/PCH, G/JE`
| $58 | `jgn`           | 2 | `PC/AB, AB/DP, MEM/DB, DB/ARL; PCI`
|     |                 | 3 | `PC/AB, AB/DP, MEM/DB, DB/ARH; PCI`
|     |                 | 4 | `AR/AB, ABL/PCL, ABH/PCH, G/JE, IJE`
| $59 | `cll`           | 2 | `SPD`
|     |                 | 3 | `SP/AB, AB/DP, PCH/DB, DB/MEM, SDP`
|     |                 | 4 | `SP/AB, AB/DP, PCL/DB, DB/MEM`
|     |                 | 5 | `PC/AB, AB/DP, MEM/DB, DB/ARL; PCI`
|     |                 | 6 | `PC/AB, AB/DP, MEM/DB, DB/ARH; PCI`
|     |                 | 7 | `AR/AB, ABL/PCL, ABH/PCH`
| $5A | `ret`           | 2 | `SP/AB, AB/DP, MEM/DB, DB/ARL; SPI`
|     |                 | 3 | `SP/AB, AB/DP, MEM/DB, DB/ARL; SPI`
|     |                 | 4 | `AR/AB, ABL/PCL, ABH/PCH`
| $5B | `rti`           | 2 | `SP/AB, AB/DP, MEM/DB, DB/S; SPI`
|     |                 | 3 | `SP/AB, AB/DP, MEM/DB, DB/ARL; SPI`
|     |                 | 4 | `SP/AB, AB/DP, MEM/DB, DB/ARH; SPI`
|     |                 | 5 | `AR/AB, ABL/PCL, ABH/PCH`
| $5C | `hlt`           | 2 | `SHF`

