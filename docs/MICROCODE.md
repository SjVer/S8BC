# Microcode

# Aliases

<!-- ALIAS_START -->
| name | signals |
| --- | --- |
| `Fetch->DB` | `PCL/ABL, PCH/ABH, AB/DP, Mem/DB, PC-En`
| `AR->PC` | `AR/AB, AB/PC`
<!-- ALIAS_END -->

## Instruction steps

<!-- CODE_START -->
| opcode | instruction  | step | control signals
| --- | --- | --- | --- |
| $XX | *any*           | 1 | `Fetch->DB, DB/IR`
| $00 | `nop`           | 2 | `End`
| $01 | `lda` immediate | 2 | `Fetch->DB, DB/A`
|     |                 | 3 | `End`
| $02 | `lda` operand X | 2 | `X/DB, DL-Lo, PCH/ABH, AB/DP`
|     |                 | 3 | `Mem/DB, DB/A`
|     |                 | 4 | `End`
| $03 | `lda` operand Y | 2 | `Y/DB, DL-Lo, PCH/ABH, AB/DP`
|     |                 | 3 | `Mem/DB, DB/A`
|     |                 | 4 | `End`
| $04 | `lda` absolute  | 2 | `Fetch->DB, DB/ARL`
|     |                 | 3 | `Fetch->DB, DB/ARH`
|     |                 | 4 | `AR/AB, AB/DP, Mem/DB, DB/A`
|     |                 | 5 | `End`
| $05 | `ldx` immediate | 2 | `Fetch->DB, DB/X`
|     |                 | 3 | `End`
| $06 | `ldx` operand Y | 2 | `Y/DB, DL-Lo, PCH/ABH, AB/DP`
|     |                 | 3 | `Mem/DB, DB/X`
|     |                 | 4 | `End`
| $07 | `ldx` absolute  | 2 | `Fetch->DB, DB/ARL`
|     |                 | 3 | `Fetch->DB, DB/ARH`
|     |                 | 4 | `AR/AB, AB/DP, Mem/DB, DB/X`
|     |                 | 5 | `End`
| $08 | `ldy` immediate | 2 | `Fetch->DB, DB/Y`
|     |                 | 3 | `End`
| $09 | `ldy` operand X | 2 | `X/DB, DL-Lo, PCH/ABH, AB/DP`
|     |                 | 3 | `Mem/DB, DB/Y`
|     |                 | 4 | `End`
| $0A | `ldy` absolute  | 2 | `Fetch->DB, DB/ARL`
|     |                 | 3 | `Fetch->DB, DB/ARH`
|     |                 | 4 | `AR/AB, AB/DP, Mem/DB, DB/Y`
|     |                 | 5 | `End`
| $0B | `ldi` absolute  | 2 | ``
|     |                 | 3 | `End`
| $0C | `sta` operand X | 2 | `X/DB, DL-Lo, PCH/ABH, AB/DP`
|     |                 | 3 | `A/DB, DB/Mem`
|     |                 | 4 | `End`
| $0D | `sta` operand Y | 2 | `Y/DB, DL-Lo, PCH/ABH, AB/DP`
|     |                 | 3 | `A/DB, DB/Mem`
|     |                 | 4 | `End`
| $0E | `sta` absolute  | 2 | `Fetch->DB, DB/ARL`
|     |                 | 3 | `Fetch->DB, DB/ARH`
|     |                 | 4 | `AR/AB, AB/DP, A/DB, DB/Mem`
|     |                 | 5 | `End`
| $0F | `stx` absolute  | 2 | `Fetch->DB, DB/ARL`
|     |                 | 3 | `Fetch->DB, DB/ARH`
|     |                 | 4 | `AR/AB, AB/DP, X/DB, DB/Mem`
|     |                 | 5 | `End`
| $10 | `sty` absolute  | 2 | `Fetch->DB, DB/ARL`
|     |                 | 3 | `Fetch->DB, DB/ARH`
|     |                 | 4 | `AR/AB, AB/DP, Y/DB, DB/Mem`
|     |                 | 5 | `End`
| $11 | `sti` absolute  | 2 | ``
|     |                 | 3 | `End`
| $12 | `tax`           | 2 | `A/DB, DB/X`
|     |                 | 3 | `End`
| $13 | `tay`           | 2 | `A/DB, DB/Y`
|     |                 | 3 | `End`
| $14 | `txa`           | 2 | `X/DB, DB/A`
|     |                 | 3 | `End`
| $15 | `tya`           | 2 | `Y/DB, DB/A`
|     |                 | 3 | `End`
| $16 | `sax`           | 2 | `A/DB, DB/R`
|     |                 | 3 | `X/DB, DB/A`
|     |                 | 4 | `R/DB, DB/X`
|     |                 | 5 | `End`
| $17 | `say`           | 2 | `A/DB, DB/R`
|     |                 | 3 | `Y/DB, DB/A`
|     |                 | 4 | `R/DB, DB/Y`
|     |                 | 5 | `End`
| $18 | `sxy`           | 2 | `X/DB, DB/R`
|     |                 | 3 | `Y/DB, DB/X`
|     |                 | 4 | `R/DB, DB/Y`
|     |                 | 5 | `End`
| $19 | `tsx`           | 2 | `SP/DB, DB/X`
|     |                 | 3 | `End`
| $1A | `txs`           | 2 | `X/DB, DB/SP`
|     |                 | 3 | `End`
| $1B | `psh` immediate | 2 | ``
|     |                 | 3 | `End`
| $1C | `psh` implied   | 2 | `SP-Dec`
|     |                 | 3 | `SP/AB, AB/DP, A/DB, DB/Mem`
|     |                 | 4 | `End`
| $1D | `psh` operand X | 2 | `SP-Dec`
|     |                 | 3 | `SP/AB, AB/DP, X/DB, DB/Mem`
|     |                 | 4 | `End`
| $1E | `psh` operand Y | 2 | `SP-Dec`
|     |                 | 3 | `SP/AB, AB/DP, Y/DB, DB/Mem`
|     |                 | 4 | `End`
| $1F | `pll`           | 2 | `SP/AB, AB/DP, Mem/DB, DB/A`
|     |                 | 3 | `End`
| $20 | `pop` implied   | 2 | `SP/AB, AB/DP, Mem/DB, DB/A, SP-Inc`
|     |                 | 3 | `End`
| $21 | `pop` operand X | 2 | `SP/AB, AB/DP, Mem/DB, DB/X, SP-Inc`
|     |                 | 3 | `End`
| $22 | `pop` operand Y | 2 | `SP/AB, AB/DP, Mem/DB, DB/Y, SP-Inc`
|     |                 | 3 | `End`
| $23 | `and` immediate | 2 | ``
|     |                 | 3 | `End`
| $24 | `and` operand X | 2 | ``
|     |                 | 3 | `End`
| $25 | `and` absolute  | 2 | ``
|     |                 | 3 | `End`
| $26 | `ior` immediate | 2 | ``
|     |                 | 3 | `End`
| $27 | `ior` operand X | 2 | ``
|     |                 | 3 | `End`
| $28 | `ior` absolute  | 2 | ``
|     |                 | 3 | `End`
| $29 | `xor` immediate | 2 | ``
|     |                 | 3 | `End`
| $2A | `xor` operand X | 2 | ``
|     |                 | 3 | `End`
| $2B | `xor` absolute  | 2 | ``
|     |                 | 3 | `End`
| $2C | `shl` immediate | 2 | ``
|     |                 | 3 | `End`
| $2D | `shl` operand X | 2 | ``
|     |                 | 3 | `End`
| $2E | `shl` absolute  | 2 | ``
|     |                 | 3 | `End`
| $2F | `shr` immediate | 2 | ``
|     |                 | 3 | `End`
| $30 | `shr` operand X | 2 | ``
|     |                 | 3 | `End`
| $31 | `shr` absolute  | 2 | ``
|     |                 | 3 | `End`
| $32 | `not` implied   | 2 | ``
|     |                 | 3 | `End`
| $33 | `not` operand X | 2 | ``
|     |                 | 3 | `End`
| $34 | `not` operand Y | 2 | ``
|     |                 | 3 | `End`
| $35 | `not` absolute  | 2 | ``
|     |                 | 3 | `End`
| $36 | `add` immediate | 2 | ``
|     |                 | 3 | `End`
| $37 | `add` operand X | 2 | ``
|     |                 | 3 | `End`
| $38 | `add` absolute  | 2 | ``
|     |                 | 3 | `End`
| $39 | `sub` immediate | 2 | ``
|     |                 | 3 | `End`
| $3A | `sub` operand X | 2 | ``
|     |                 | 3 | `End`
| $3B | `sub` absolute  | 2 | ``
|     |                 | 3 | `End`
| $3C | `mul` immediate | 2 | ``
|     |                 | 3 | `End`
| $3D | `mul` operand X | 2 | ``
|     |                 | 3 | `End`
| $3E | `mul` absolute  | 2 | ``
|     |                 | 3 | `End`
| $3F | `div` immediate | 2 | ``
|     |                 | 3 | `End`
| $40 | `div` operand X | 2 | ``
|     |                 | 3 | `End`
| $41 | `div` absolute  | 2 | ``
|     |                 | 3 | `End`
| $42 | `inc` implied   | 2 | ``
|     |                 | 3 | `End`
| $43 | `inc` operand X | 2 | ``
|     |                 | 3 | `End`
| $44 | `inc` operand Y | 2 | ``
|     |                 | 3 | `End`
| $45 | `inc` absolute  | 2 | ``
|     |                 | 3 | `End`
| $46 | `dec` implied   | 2 | ``
|     |                 | 3 | `End`
| $47 | `dec` operand X | 2 | ``
|     |                 | 3 | `End`
| $48 | `dec` operand Y | 2 | ``
|     |                 | 3 | `End`
| $49 | `dec` absolute  | 2 | ``
|     |                 | 3 | `End`
| $4A | `cmp` immediate | 2 | ``
|     |                 | 3 | `End`
| $4B | `cmp` operand X | 2 | ``
|     |                 | 3 | `End`
| $4C | `cmp` operand Y | 2 | ``
|     |                 | 3 | `End`
| $4D | `cmp` absolute  | 2 | ``
|     |                 | 3 | `End`
| $4E | `jmp`           | 2 | `Fetch->DB, DB/ARL`
|     |                 | 3 | `Fetch->DB, DB/ARH`
|     |                 | 4 | `AR->PC`
|     |                 | 5 | `End`
| $4F | `jzs`           | 2 | `Fetch->DB, DB/ARL`
|     |                 | 3 | `Fetch->DB, DB/ARH`
<!-- |     |                 | 4 | `AR->PC, Z/JE` -->
|     |                 | 5 | `End`
| $50 | `jzn`           | 2 | `Fetch->DB, DB/ARL`
|     |                 | 3 | `Fetch->DB, DB/ARH`
<!-- |     |                 | 4 | `AR->PC, Z/JE, JE-Inv` -->
|     |                 | 5 | `End`
| $51 | `jcs`           | 2 | `Fetch->DB, DB/ARL`
|     |                 | 3 | `Fetch->DB, DB/ARH`
<!-- |     |                 | 4 | `AR->PC, C/JE` -->
|     |                 | 5 | `End`
| $52 | `jcn`           | 2 | `Fetch->DB, DB/ARL`
|     |                 | 3 | `Fetch->DB, DB/ARH`
<!-- |     |                 | 4 | `AR->PC, C/JE, JE-Inv` -->
|     |                 | 5 | `End`
| $53 | `jes`           | 2 | `Fetch->DB, DB/ARL`
|     |                 | 3 | `Fetch->DB, DB/ARH`
<!-- |     |                 | 4 | `AR->PC, E/JE` -->
|     |                 | 5 | `End`
| $54 | `jen`           | 2 | `Fetch->DB, DB/ARL`
|     |                 | 3 | `Fetch->DB, DB/ARH`
<!-- |     |                 | 4 | `AR->PC, E/JE, JE-Inv` -->
|     |                 | 5 | `End`
| $55 | `jls`           | 2 | `Fetch->DB, DB/ARL`
|     |                 | 3 | `Fetch->DB, DB/ARH`
<!-- |     |                 | 4 | `AR->PC, L/JE` -->
|     |                 | 5 | `End`
| $56 | `jln`           | 2 | `Fetch->DB, DB/ARL`
|     |                 | 3 | `Fetch->DB, DB/ARH`
<!-- |     |                 | 4 | `AR->PC, L/JE, JE-Inv` -->
|     |                 | 5 | `End`
| $57 | `jgs`           | 2 | `Fetch->DB, DB/ARL`
|     |                 | 3 | `Fetch->DB, DB/ARH`
<!-- |     |                 | 4 | `AR->PC, G/JE` -->
|     |                 | 5 | `End`
| $58 | `jgn`           | 2 | `Fetch->DB, DB/ARL`
|     |                 | 3 | `Fetch->DB, DB/ARH`
<!-- |     |                 | 4 | `AR->PC, G/JE, JE-Inv` -->
|     |                 | 5 | `End`
| $59 | `cll`           | 2 | `SP-Dec`
|     |                 | 3 | `SP/AB, AB/DP, PCH/DB, DB/Mem, SP-Dec`
|     |                 | 4 | `SP/AB, AB/DP, PCL/DB, DB/Mem`
|     |                 | 5 | `Fetch->DB, DB/ARL`
|     |                 | 6 | `Fetch->DB, DB/ARH`
|     |                 | 7 | `AR->PC`
| $5A | `ret`           | 2 | `SP/AB, AB/DP, Mem/DB, DB/ARL, SP-Inc`
|     |                 | 3 | `SP/AB, AB/DP, Mem/DB, DB/ARL, SP-Inc`
|     |                 | 4 | `AR->PC`
|     |                 | 5 | `End`
| $5B | `rti`           | 2 | `SP/AB, AB/DP, Mem/DB, DB/SR, SP-Inc`
|     |                 | 3 | `SP/AB, AB/DP, Mem/DB, DB/ARL, SP-Inc`
|     |                 | 4 | `SP/AB, AB/DP, Mem/DB, DB/ARH, SP-Inc`
|     |                 | 5 | `AR->PC`
|     |                 | 6 | `End`
| $5C | `hlt`           | 2 | `HF-Set`
|     |                 | 3 | `End`
<!-- CODE_END -->
