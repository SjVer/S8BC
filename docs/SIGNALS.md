# Control Signals

<!-- SIG_START -->
| bit | name | width | comment |
| --- | --- | --- | --- |
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