TTY_OUTPUT = $0202
CURR_FIB_COUNT = $0001
FIB_COUNT = $0002

reset:
reset_loop:
  jmp reset_loop


fib:
  ; handle 1st and 2nd case
  cmp #0
  jes fib_end_0
  cmp #1
  jes fib_end_1
  ; store counter - 1
  dec
  sta FIB_COUNT
  ; set up terms
  ldy #0
  ldx #1
  lda #1
fib_loop:
  ; check if done
  dec FIB_COUNT
  cmp FIB_COUNT
  jzs fib_end
  ; move terms
  psh x
  pop y
  tax
  ; set next term
  tya
  add x
  jmp fib_loop
fib_end_0:
  lda #0
  ret
fib_end_1:
  lda #1
  ret
fib_end:
  ret
  


print_int:
  ; if (a < 10) print char
  cmp #10
  jls print_int_putchar
  ; else recurse
  psh
  div #10
  cll print_int
  pop
print_int_putchar:
  ; find remainder
  psh
  div #10
  mul #10
  tax
  pop
  sub x
  ; convert to ascii and print
  add #48 ; '0'
  sta TTY_OUTPUT
  ret



input:
  lda #0
  sta CURR_FIB_COUNT
input_loop:
  ; print the label
  lda CURR_FIB_COUNT
  cll print_int
  lda #58 ; ':'
  sta TTY_OUTPUT
  lda #32 ; ' '
  sta TTY_OUTPUT
  ; calc nth fib number
  lda CURR_FIB_COUNT
  cll fib
  ; print it
  cll print_int
  lda #10 ; '\n'
  sta TTY_OUTPUT
  lda #13 ; '\r'
  sta TTY_OUTPUT
  ; stop after 13
  inc CURR_FIB_COUNT
  lda #14
  cmp CURR_FIB_COUNT
  jen input_loop
  rti


$fffc: .word #input
$fffe: .word #reset
