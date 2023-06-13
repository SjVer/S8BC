TTY_INPUT = $0201
TTY_OUTPUT = $0202

reset:
  jmp reset

input:
  lda TTY_INPUT
  sta TTY_OUTPUT
  
  sub #13
  jnz input_done
  
  lda #$0a
  sta TTY_OUTPUT
input_done:
  rti

$fffc: .word #input
$fffe: .word #reset
