# Instructions

## Load/Store Operations

These instructions transfer a single byte between memory and one of the registers.
Load operations can load an immediate byte or a byte from memory.
The `ldi` and `sti` instructions operate on an address stored in memory.
The `sta` instruction can store at an address composed of the lower 8 bits of PC and X or Y as higher 8 bits.   

`lda`: Loads a byte into A

`ldx`: Load a byte into X

`ldy`: Load a byte into Y

`ldi`: Indirectly load a byte into A

`sta`: Store A in memory

`stx`: Store X in memory

`sty`: Store Y in memory

`sti`: Indirectly store A in memory.

## Register Operations

These instructions transfer and swap the values of registers.

`tax`: Transfer A to X 

`tay`: Transfer A to Y

`txa`: Transfer X to A 

`tya`: Transfer Y to A 

`sax`: Swap A and X 

`say`: Swap A and Y 

`sxy`: Swap X and Y

## Stack Operations

These instructions operate on the stack and SP.
The `psh` instruction can push an immediate byte or one of the registers.
The `pll` instruction does not remove a byte off the stack, but `pop` does.

`tsx`: Transfer SP to X 

`txs`: Transfer X to SP 

`psh`: Push onto the stack 

`pll`: Pull from the stack 

`pop`: Pop from the stack

## Bitwise Operations 

These instructions peform bitwise operations on an immediate byte, X or a byte at an address.
All these instructions store their results in A and set the C and Z.

`and`: Bitwise AND A and a byte

`ior`: Bitwise inclusive OR A and a byte

`xor`: Bitwise exclusive OR A and a byte

`shl`: Shift the bits of A to the left

`shr`: Shift the bits of A to the right

`not`: Bitwise NOT A

## Numerical Operations

These instructions peform arithmetical operations on an immediate byte, X or a byte at an address.
The `add`, `sub`, `mul` and `div` instructions store their results in A.
The `inc` and `dec` instructions operate on A, X, Y or a byte in memory.
All these instructions set C and Z according to the result.

`add`: Add A and a byte

`sub`: Subtract A and a byte

`mul`: Multiply A and a byte

`div`: Divide A and a byte

`inc`: Increment a byte 

`dec`: Decrement a byte

`cmp`: Compare A and a byte and set E, L and G.

## Control Flow Operations

These instructions change the control flow of execution.
All jump instructions operate on an immediate 16-bit address.
The `cll` instruction calls by pushing PC onto the stack, the `ret` instruction does the opposite.
The `rti` instruction acts the same as `ret` but also pops the flags.

`jmp`: Jump to an address

`jzs`: Jump to an address if Z is set

`jzn`: Jump to an address if Z is not set

`jcs`: Jump to an address if C is set

`jcn`: Jump to an address if C is not set

`jes`: Jump to an address if E is set

`jen`: Jump to an address if E is not set

`jls`: Jump to an address if L is set

`jln`: Jump to an address if L is not set

`jgs`: Jump to an address if G is set

`jgn`: Jump to an address if G is not set

`cll`: Call a subroutine at an address

`ret`: Return from a subroutine 

`rti`: Return from an interrupt

`hlt`: Halt execution

`nop`: No operation 

