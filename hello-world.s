  .org $8000 ; the following code begins at address $8000
.include "hardware.s"
.include "lcd.s"
string:
  .asciiz "  Hello World"
reset:
  ldx #$ff
  txs

  lda #%11111111 ; Set all pins on port B to output
  sta DDRB
  lda #%11100000 ; Set top 3 pins on port A to output
  sta DDRA

  lda #%00111000 ; 8-bit mode, 2-line display, 5x8 font
  jsr lcd_instruction
  lda #%00001110 ; Display on, cursor on, blink off
  jsr lcd_instruction
  lda #%00000110 ; Increment, don't shift display
  jsr lcd_instruction
  lda #%00000001 ; Return home
  jsr lcd_instruction
  lda #%00000001 ; Clear display
  jsr lcd_instruction

  ldx #0  
print_loop:
  lda string,x 
  beq loop
  jsr print_char
  inx
  jmp print_loop

loop:
  jmp loop
  .org $fffc ;the following code begins at address $fffc
  .word reset
  .word $0000

