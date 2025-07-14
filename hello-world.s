  .org $8000
.include "lcd.s"

reset:
  lda #LCD_BOTTOM
  sta V_LCD_SELECT
  jsr lcd_init

  lda #LCD_TOP
  sta V_LCD_SELECT
  jsr lcd_init

  lda #'H'
  jsr lcd_char
  lda #'e'
  jsr lcd_char
  lda #'l'
  jsr lcd_char
  lda #'l'
  jsr lcd_char
  lda #'o'
  jsr lcd_char

  lda #LCD_BOTTOM
  sta V_LCD_SELECT

  lda #'W'
  jsr lcd_char
  lda #'o'
  jsr lcd_char
  lda #'r'
  jsr lcd_char
  lda #'l'
  jsr lcd_char
  lda #'d'
  jsr lcd_char

loop:
  jmp loop

.include "vectors.s"
