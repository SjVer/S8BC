# Control Signals

<!-- SIG_START -->
| bit | name | width | comment |
| --- | --- | --- | --- |
| 0  | DB/IR    | 1 | 
| 1  | DB/Mem   | 1 | 
| 2  | Mem/DB   | 1 | 
| 3  | AB/DP    | 1 | 
| 4  | DB/ARL   | 1 | 
| 5  | DB/ARH   | 1 | 
| 6  | AR/AB    | 1 | 
| 7  | AB/PC    | 1 | 
| 8  | PC-En    | 1 | Enable PC
| 9  | PCL/ABL  | 1 | 
| 10 | PCH/ABH  | 1 | 
| 11 | PCL/DB   | 1 | 
| 12 | PCH/DB   | 1 | 
| 13 | JE-Inv   | 1 | Invert JE
| 14 | DL-Lo    | 1 | Connect DB and ABL
| 15 | DL-Hi    | 1 | Connect DB and ABH
| 16 | DB/R     | 1 | 
| 17 | R/DB     | 1 | 
| 18 | ALU/R    | 1 | 
| 19 | ALU-Sel  | 4 | Select ALU operation
| 23 | ALU/A    | 1 | 
| 24 | DB/A     | 1 | 
| 25 | A/DB     | 1 | 
| 26 | SP-Inc   | 1 | Increment SP
| 27 | SP-Dec   | 1 | Decrement SP
| 28 | DB/SP    | 1 | 
| 29 | SP/DB    | 1 | 
| 30 | SP/AB    | 1 | 
| 31 | DB/X     | 1 | 
| 32 | X/DB     | 1 | 
| 33 | DB/Y     | 1 | 
| 34 | Y/DB     | 1 | 
| 35 | DB/SR    | 1 | 
| 36 | SR/DB    | 1 | 
| 37 | SR-Sel   | 3 | Select flag for output to JE
| 40 | HF-Set   | 1 | Set H flag
| 41 | End      | 1 | Reset step counter
<!-- SIG_END -->

## ALU operation selection

| ALU-Sel | operation |
| --- | --- |
| $0 | No operation 
| $1 | Add 
| $2 | Subtract 
| $3 | Multiply
| $4 | Divide 
| $5 | Binary AND 
| $6 | Binary OR 
| $7 | Binary XOR 
| $8 | Logical shift left 
| $9 | Logical shift right 
| $a | Binary NOT 
| $b | Increment 
| $c | Decrement 

## Flag output selection

| SR-Sel | flag |
| --- | --- |
| $0 | Z
| $1 | C
| $2 | E
| $3 | L
| $4 | G
| $6 | I
| $7 | H
