start:
  ldx #message ; char ptr

loop:
  lda x ; load the next char
  jzs wait ; end if char is 0

  sta $0202 ; write the char
  inc x ; incr the char ptr

  jmp loop

wait:
  lda $0201 ; escape key status
  jzs wait
  hlt


message:
  .string "Hello, World!"
  .byte #0

; reset vector
$fffe:
  .word #start
