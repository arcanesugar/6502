.include "vectors.s"
PORTB = $6000
PORTA = $6001
DDRB = $6002
DDRA = $6003

LCD_E  = %10000000
LCD_RW = %01000000
LCD_RS = %00100000

  .org $8000
reset:
  lda #%11111111
  sta DDRA
  lda #%11111111
  sta DDRB
  
  lda #%00101000 ; 4-bit; 2-lines; 5x8
  jsr lcd_instruction
  lda #%00001111 ; Display on; cursor on; blink on
  jsr lcd_instruction
  lda #%00000110 ; Shift cursor
  jsr lcd_instruction
  lda #$00000001 ; Clear display
  jsr lcd_instruction

loop:
  jmp loop

lcd_instruction:
  tax
  lda #%11111111
  sta DDRA
  sta DDRB
  txa
  
  ;store top of instruction in x
  tay ;stash instruction in y
  and #%11110000
  tax
  tya

  ;store bottom of instruction in y
  and #%00001111
  asl
  asl
  asl
  asl
  tay
  
  txa
  sta PORTB
  lda #0
  sta PORTA
  lda #LCD_E
  sta PORTA
  lda #0
  sta PORTA
  
  tya
  sta PORTB
  lda #0
  sta PORTA
  lda #LCD_E
  sta PORTA
  lda #0
  sta PORTA

  rts
