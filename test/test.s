STATUS_CHAR = $0300
KEYCODE_CHAR = $031f
KEYCODE_ADDR = $0202

reset:
  lda #$5f ; '_'
  ldx #$20 ; ' '
  sta STATUS_CHAR
  sta KEYCODE_CHAR

loop:
  ; sta STATUS_CHAR
  ; sax
  jmp loop

input:
  rti
  ; save reg
  psh

  ; print the keycode
  lda KEYCODE_ADDR
  add #86 ; keycode to ascii
  sta KEYCODE_CHAR

  ; restore reg and return
  pop
  rti

$fffc: .word #input
$fffe: .word #reset