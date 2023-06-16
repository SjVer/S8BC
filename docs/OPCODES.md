# Opcodes

| opcode | instruction | mode |
| --- | --- | --- |
| $00 | nop |  |
| $01 | lda | immediate |
| $02 | lda | operand X |
| $03 | lda | operand Y |
| $04 | lda | absolute |
| $05 | ldx | immediate |
| $06 | ldx | operand Y |
| $07 | ldx | absolute |
| $08 | ldy | immediate |
| $09 | ldy | operand X |
| $0a | ldy | absolute |
| $0b | ldi | absolute |
| $0c | sta | operand X |
| $0d | sta | operand Y |
| $0e | sta | absolute |
| $0f | stx | absolute |
| $10 | sty | absolute |
| $11 | sti | absolute |
| $12 | tax |  |
| $13 | tay |  |
| $14 | txa |  |
| $15 | tya |  |
| $16 | sax |  |
| $17 | say |  |
| $18 | sxy |  |
| $19 | tsx |  |
| $1a | txs |  |
| $1b | psh | immediate |
| $1c | psh | implied |
| $1d | psh | operand X |
| $1e | psh | operand Y |
| $1f | pll |  |
| $20 | pop | implied |
| $21 | pop | operand X |
| $22 | pop | operand Y |
| $23 | and | immediate |
| $24 | and | operand X |
| $25 | and | absolute |
| $26 | ior | immediate |
| $27 | ior | operand X |
| $28 | ior | absolute |
| $29 | xor | immediate |
| $2a | xor | operand X |
| $2b | xor | absolute |
| $2c | shl | immediate |
| $2d | shl | operand X |
| $2e | shl | absolute |
| $2f | shr | immediate |
| $30 | shr | operand X |
| $31 | shr | absolute |
| $32 | not | implied |
| $33 | not | operand X |
| $34 | not | operand Y |
| $35 | not | absolute |
| $36 | add | immediate |
| $37 | add | operand X |
| $38 | add | absolute |
| $39 | sub | immediate |
| $3a | sub | operand X |
| $3b | sub | absolute |
| $3c | mul | immediate |
| $3d | mul | operand X |
| $3e | mul | absolute |
| $3f | div | immediate |
| $40 | div | operand X |
| $41 | div | absolute |
| $42 | inc | implied |
| $43 | inc | operand X |
| $44 | inc | operand Y |
| $45 | inc | absolute |
| $46 | dec | implied |
| $47 | dec | operand X |
| $48 | dec | operand Y |
| $49 | dec | absolute |
| $4a | cmp | immediate |
| $4b | cmp | operand X |
| $4c | cmp | operand Y |
| $4d | cmp | absolute |
| $4e | jmp |  |
| $4f | jzs |  |
| $50 | jzn |  |
| $51 | jcs |  |
| $52 | jcn |  |
| $53 | jes |  |
| $54 | jen |  |
| $55 | jls |  |
| $56 | jln |  |
| $57 | jgs |  |
| $58 | jgn |  |
| $59 | cll |  |
| $5a | ret |  |
| $5b | rti |  |
| $5c | hlt |  |
<!-- END OPCODES -->
