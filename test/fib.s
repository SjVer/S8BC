TTY_OUTPUT = $0202

reset:
reset_loop:
  jmp reset_loop



print_int:
  psh  
  ; if (a <= 9) print char
  sub #9
  hlt
  jnz print_int_putchar
  jic print_int_putchar
  ; else recurse
  pop
  div #10
  cll print_int
  ret
print_int_putchar:
  pop
  add #48 ; '0'
  sta TTY_OUTPUT
  ret



input:
  lda #123 
  cll print_int
  lda #10 ; '\n'
  sta TTY_OUTPUT
  rti


$fffc: .word #input
$fffe: .word #reset
