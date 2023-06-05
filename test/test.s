  ; .byte 255
  ; .word 32768

; start execution
start:
  lda #1
loop:
  add #1
  jmp loop

65533:
  .word start