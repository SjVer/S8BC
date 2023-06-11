start:
  ldx #message ; char ptr

  ; store the buff ptr at $00
  lda #$00
  sta $00
  lda #$03
  sta $01

loop:
  lda x ; load the next char
  jiz end ; end if char is 0

  sti $00 ; write the char
  inc x ; incr the char ptr
  inc $00 ; incr the buff ptr

  jmp loop

end:
  hlt


message:
  .string "Hello, World!"
  .byte #0

; reset vector
$fffe:
  .word #start