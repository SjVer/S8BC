start:
  lda #97 ; 'a'
  ldx #$0A ; ptr

loop:
  sta x ; store char at ptr
  inc ; incr char
  dec x; decr ptr
  jiz end

end:
  hlt

; reset vector
$fffc:
  .word #start