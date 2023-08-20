# Opcodes

| opcode | instruction | mode |
| --- | --- | --- |
| $00 | `nop` |  |
| $01 | `lda` | immediate |
| $02 | `lda` | operand X |
| $03 | `lda` | operand Y |
| $04 | `lda` | stack |
| $05 | `lda` | absolute |
| $06 | `ldx` | immediate |
| $07 | `ldx` | operand Y |
| $08 | `ldx` | stack |
| $09 | `ldx` | absolute |
| $0A | `ldy` | immediate |
| $0B | `ldy` | operand X |
| $0C | `ldy` | stack |
| $0D | `ldy` | absolute |
| $0E | `ldi` | absolute |
| $0F | `sta` | operand X |
| $10 | `sta` | operand Y |
| $11 | `sta` | stack |
| $12 | `sta` | absolute |
| $13 | `stx` | absolute |
| $14 | `sty` | absolute |
| $15 | `sti` | absolute |
| $16 | `tax` |  |
| $17 | `tay` |  |
| $18 | `txa` |  |
| $19 | `tya` |  |
| $1A | `sax` |  |
| $1B | `say` |  |
| $1C | `sxy` |  |
| $1D | `tsx` |  |
| $1E | `txs` |  |
| $1F | `psh` | immediate |
| $20 | `psh` | implied |
| $21 | `psh` | operand X |
| $22 | `psh` | operand Y |
| $23 | `pll` |  |
| $24 | `pop` | implied |
| $25 | `pop` | operand X |
| $26 | `pop` | operand Y |
| $27 | `and` | immediate |
| $28 | `and` | operand X |
| $29 | `and` | absolute |
| $2A | `ior` | immediate |
| $2B | `ior` | operand X |
| $2C | `ior` | absolute |
| $2D | `xor` | immediate |
| $2E | `xor` | operand X |
| $2F | `xor` | absolute |
| $30 | `shl` | immediate |
| $31 | `shl` | operand X |
| $32 | `shl` | absolute |
| $33 | `shr` | immediate |
| $34 | `shr` | operand X |
| $35 | `shr` | absolute |
| $36 | `not` | implied |
| $37 | `not` | operand X |
| $38 | `not` | operand Y |
| $39 | `not` | absolute |
| $3A | `add` | immediate |
| $3B | `add` | operand X |
| $3C | `add` | absolute |
| $3D | `sub` | immediate |
| $3E | `sub` | operand X |
| $3F | `sub` | absolute |
| $40 | `mul` | immediate |
| $41 | `mul` | operand X |
| $42 | `mul` | absolute |
| $43 | `div` | immediate |
| $44 | `div` | operand X |
| $45 | `div` | absolute |
| $46 | `inc` | implied |
| $47 | `inc` | operand X |
| $48 | `inc` | operand Y |
| $49 | `inc` | absolute |
| $4A | `dec` | implied |
| $4B | `dec` | operand X |
| $4C | `dec` | operand Y |
| $4D | `dec` | absolute |
| $4E | `cmp` | immediate |
| $4F | `cmp` | operand X |
| $50 | `cmp` | operand Y |
| $51 | `cmp` | absolute |
| $52 | `jmp` |  |
| $53 | `jzs` |  |
| $54 | `jzn` |  |
| $55 | `jcs` |  |
| $56 | `jcn` |  |
| $57 | `jes` |  |
| $58 | `jen` |  |
| $59 | `jls` |  |
| $5A | `jln` |  |
| $5B | `jgs` |  |
| $5C | `jgn` |  |
| $5D | `cll` |  |
| $5E | `ret` |  |
| $5F | `rti` |  |
| $60 | `hlt` |  |
<!-- END OPCODES -->
