  .org $8000
.include "lcd.s"

reset:
  lda #-1
  sta DDRB
  
  jsr lcd_init

  lda #%00001111 ; Display on; cursor on; blink on
  jsr lcd_instruction
  lda #%00000110 ; Shift cursor
  jsr lcd_instruction
  lda #$00000001 ; Clear display
  jsr lcd_instruction

  lda #'H'
  jsr lcd_char
  lda #'i'
  jsr lcd_char
  lda #' '
  jsr lcd_char
  lda #'>'
  jsr lcd_char
  lda #':'
  jsr lcd_char
  lda #'{'
  jsr lcd_char

  lda #STATUS_LIGHT_MASK
  sta PORTB
loop:
  jmp loop

.include "vectors.s"
