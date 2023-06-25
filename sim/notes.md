# Control Signals

| bit | name | operation | edge |
| --- | --- | --- | --- |
|  | CLB | connect DB and LB |
|  | CHB | connect DB and HB |
| <!-- --> | <!-- --> | <!-- --> |
|  | PCE | enable PC | F
|  | PCI | PC in from LB and HB | R
|  | PCO | PC out to LB and HB | R
| <!-- --> | <!-- --> | <!-- --> |
|  | ADI | address in from LB and HB | R
|  | ADO | address out to LB and HB | R
|  | ALI | address low in from DB | R
|  | AHI | address high in from DB | R
|  | MAI | memory address in from LB and HB | R
|  | MEI | memory in from DB | R
|  | MEO | memory out to DB | R
| <!-- --> | <!-- --> | <!-- --> |
|  | SPI | SP in from DB | R
|  | SPO | SP out to DB | R
|  | SPA | SP out to LB and HB | R
|  | IRI | IR in from DB | R
| <!-- --> | <!-- --> | <!-- --> |
|  | ARI | A in from DB | R
|  | ARO | A out to DB | R
|  | XRI | X in from DB | R
|  | XRO | X out to DB | R
|  | YRI | Y in from DB | R
|  | XRO | Y out to DB | R
| <!-- --> | <!-- --> | <!-- --> |
|  | AAD | ALU add | R
|  | ASU | ALU subtract | R
|  | AMU | ALU multiply | R
|  | ADI | ALU divide | R
|  | AAN | ALU bitwise and | R
|  | AIO | ALU bitwise or | R
|  | AXO | ALU bitwise xor | R
|  | ASL | ALU shift left | R
|  | ASR | ALU shift right | R
|  | ANO | ALU bitwise not | R
|  | AIN | ALU increment | R
|  | ADE | ALU decrement | R
|  | ACM | ALU compare | R
|  | ALO | ALU out to DB | R
| <!-- --> | <!-- --> | <!-- --> |
|  | SRI | SR in from DB | R
|  | SRO | SR out to DB | R
|  | SRF | set result flags | R
|  | SCF | set comparison flags | R
|  | SIF | set interrupt flag | R
|  | CIF | clear interrupt flag | R
|  | SHF | set halt flag | R
|  | ZFO | zero flag out to JE | R
|  | CFO | carry flag out to JE | R
|  | EFO | equal flag out to JE | R
|  | LFO | lesser flag out to JE | R
|  | GFO | greater flag out to JE | R
|  | IFO | interrupt flag out to JE | R

# Instruction Steps

Step 1 is always `PCO,MAI,MEO,IRI;PCE`.

| opcode | instruction  | step 2 | step 3 | step 4 |
| --- | ---             | --- | --- | --- |
| $00 | `nop`           |
| $01 | `lda` immediate | PCO,MAI,MEO,ARI;PCE
| $02 | `lda` operand X | PCO,ADI | XRO,ALI | ADO,MAI,MEO,ARI
| $03 | `lda` operand Y | PCO,ADI | YRO,ALI | ADO,MAI,MEO,ARI
| $04 | `lda` absolute  | PCO,MAI,MEO,ALI;PCE | PCO,MAI,MEO,AHI;PCE | ADO,MAI,MEO,ARI
| $05 | `ldx` immediate | PCO,MAI,MEO,XRI;PCE
| $06 | `ldx` operand Y | PCO,ADI | YRO,ALI | ADO,MAI,MEO,XRI 
| $07 | `ldx` absolute  | PCO,MAI,MEO,ALI;PCE | PCO,MAI,MEO,AHI;PCE | ADO,MAI,MEO,XRI
| $08 | `ldy` immediate | PCO,MAI,MEO,YRI;PCE
| $09 | `ldy` operand X | PCO,ADI | XRO,ALI | ADO,MAI,MEO,YRI
| $0A | `ldy` absolute  | PCO,MAI,MEO,ALI;PCE | PCO,MAI,MEO,AHI;PCE | ADO,MAI,MEO,YRI
| $0B | `ldi` absolute  | 
| $0C | `sta` operand X | PCO,ADI | XRO,ALI | ADO,MAI,ARO,MEI
| $0D | `sta` operand Y | PCO,ADI | YRO,ALI | ADO,MAI,ARO,MEI
| $0E | `sta` absolute  | PCO,MAI,MEO,ALI;PCE | PCO,MAI,MEO,AHI;PCE | ADO,MAI,ARO,MEI
| $0F | `stx` absolute  | PCO,MAI,MEO,ALI;PCE | PCO,MAI,MEO,AHI;PCE | ADO,MAI,XRO,MEI
| $10 | `sty` absolute  | PCO,MAI,MEO,ALI;PCE | PCO,MAI,MEO,AHI;PCE | ADO,MAI,YRO,MEI
| $11 | `sti` absolute  |
| $12 | `tax`           | ARO,XRI
| $13 | `tay`           | ARO,YRI
| $14 | `txa`           | XRO,ARI
| $15 | `tya`           | YRO,ARI
| $16 | `sax`           | 
| $17 | `say`           |
| $18 | `sxy`           |
| $19 | `tsx`           | SRO,XRI
| $1A | `txs`           | XRO,SRI
| $1B | `psh` immediate | 
| $1C | `psh` implied   | SPO,ADE,ALO,SPI | SPA,MAI,ARO,MEI
| $1D | `psh` operand X | SPO,ADE,ALO,SPI | SPA,MAI,XRO,MEI
| $1E | `psh` operand Y | SPO,ADE,ALO,SPI | SPA,MAI,YRO,MEI
| $1F | `pll`           | SPA,MAI,MEI,ARO
| $20 | `pop` implied   | SPA,MAI,MEI,ARO | SPO,AIN,ALO,SPI
| $21 | `pop` operand X | SPA,MAI,MEI,XRO | SPO,AIN,ALO,SPI
| $22 | `pop` operand Y | SPA,MAI,MEI,YRO | SPO,AIN,ALO,SPI
| $23 | `and` immediate | PCO,MAI,MEO,AAN,ALO,ARI;PCE
| $24 | `and` operand X | XRO,AAN,ALO,ARI
| $25 | `and` absolute  | PCO,MAI,MEO,ALI;PCE | PCO,MAI,MEO,AHI;PCE | ADO,MAI,MEO,AAN,ALO,ARI
| $26 | `ior` immediate | PCO,MAI,MEO,AIO,ALO,ARI;PCE
| $27 | `ior` operand X | XRO,AIO,ALO,ARI
| $28 | `ior` absolute  | PCO,MAI,MEO,ALI;PCE | PCO,MAI,MEO,AHI;PCE | ADO,MAI,MEO,AIO,ALO,ARI
| $29 | `xor` immediate | PCO,MAI,MEO,AXO,ALO,ARI;PCE
| $2A | `xor` operand X | XRO,AXO,ALO,ARI
| $2B | `xor` absolute  | PCO,MAI,MEO,ALI;PCE | PCO,MAI,MEO,AHI;PCE | ADO,MAI,MEO,AXO,ALO,ARI
| $2C | `shl` immediate | PCO,MAI,MEO,ASL,ALO,ARI;PCE
| $2D | `shl` operand X | XRO,ASL,ALO,ARI
| $2E | `shl` absolute  | PCO,MAI,MEO,ALI;PCE | PCO,MAI,MEO,AHI;PCE | ADO,MAI,MEO,ASL,ALO,ARI
| $2F | `shr` immediate | PCO,MAI,MEO,ASR,ALO,ARI;PCE
| $30 | `shr` operand X | XRO,ASR,ALO,ARI
| $31 | `shr` absolute  | PCO,MAI,MEO,ALI;PCE | PCO,MAI,MEO,AHI;PCE | ADO,MAI,MEO,ASR,ALO,ARI
| $32 | `not` implied   | ARO,ANO,ALO,ARI
| $33 | `not` operand X | XRO,ANO,ALO,XRI
| $34 | `not` operand Y | YRO,ANO,ALO,YRI
| $35 | `not` absolute  | PCO,MAI,MEO,ALI;PCE | PCO,MAI,MEO,AHI;PCE | ADO,MAI,MEO,ANO,ALO,MEI
| $36 | `add` immediate | PCO,MAI,MEO,AAD,ALO,ARI;PCE
| $37 | `add` operand X | XRO,AAD,ALO,ARI
| $38 | `add` absolute  | PCO,MAI,MEO,ALI;PCE | PCO,MAI,MEO,AHI;PCE | ADO,MAI,MEO,AAD,ALO,ARI
| $39 | `sub` immediate | PCO,MAI,MEO,ASU,ALO,ARI;PCE
| $3A | `sub` operand X | XRO,ASU,ALO,ARI
| $3B | `sub` absolute  | PCO,MAI,MEO,ALI;PCE | PCO,MAI,MEO,AHI;PCE | ADO,MAI,MEO,ASU,ALO,ARI
| $3C | `mul` immediate | PCO,MAI,MEO,AMU,ALO,ARI;PCE
| $3D | `mul` operand X | XRO,AMU,ALO,ARI
| $3E | `mul` absolute  | PCO,MAI,MEO,ALI;PCE | PCO,MAI,MEO,AHI;PCE | ADO,MAI,MEO,AMU,ALO,ARI
| $3F | `div` immediate | PCO,MAI,MEO,ADI,ALO,ARI;PCE
| $40 | `div` operand X | XRO,ADI,ALO,ARI
| $41 | `div` absolute  | PCO,MAI,MEO,ALI;PCE | PCO,MAI,MEO,AHI;PCE | ADO,MAI,MEO,ADI,ALO,ARI
| $42 | `inc` implied   | ARO,AIN,ALO,ARI
| $43 | `inc` operand X | XRO,AIN,ALO,XRI
| $44 | `inc` operand Y | YRO,AIN,ALO,YRI
| $45 | `inc` absolute  | PCO,MAI,MEO,ALI;PCE | PCO,MAI,MEO,AHI;PCE | ADO,MAI,MEO,AIN,ALO,MEI
| $46 | `dec` implied   | ARO,ADE,ALO,ARI
| $47 | `dec` operand X | XRO,ADE,ALO,XRI
| $48 | `dec` operand Y | YRO,ADE,ALO,YRI
| $49 | `dec` absolute  | PCO,MAI,MEO,ALI;PCE | PCO,MAI,MEO,AHI;PCE | ADO,MAI,MEO,ADE,ALO,MEI
| $4A | `cmp` immediate | PCO,MAI,MEO,ACM,SCF;PCE
| $4B | `cmp` operand X | XRO,ACM,SCF
| $4C | `cmp` operand Y | YRO,ACM,SCF
| $4D | `cmp` absolute  | PCO,MAI,MEO,ALI;PCE | PCO,MAI,MEO,AHI;PCE | ADO,MAI,MEO,ACM,SCF
| $4E | `jmp`           | PCO,MAI,MEO,ALI;PCE | PCO,MAI,MEO,AHI;PCE | ADO,PCI
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
| $5C | `hlt`           | SHF

