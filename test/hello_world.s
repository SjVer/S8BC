start:
  ldx #message ; char ptr

  ; store the buff ptr at $00
  lda #$00
  sta $00
  lda #$03
  sta $01

  lda #0
  sta $0201

loop:
  lda x ; load the next char
  jiz wait ; end if char is 0

  sti $00 ; write the char
  inc x ; incr the char ptr
  inc $00 ; incr the buff ptr

  jmp loop

wait:
  lda $0201 ; escape key status
  jiz wait
  hlt


message:
  .string "Hello, World!"
  .byte #0

; reset vector
$fffe:
  .word #start