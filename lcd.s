.include "via.s"

; constants
LCD_DATA_MASK     = %11110000
LCD_RW_MASK       = %00001000
LCD_RS_MASK       = %00000100

LCD_TOP      = %10000000
LCD_BOTTOM   = %01000000

; variables
V_LCD_SELECT = $0000

lcd_init:
  ; Function set, 4 bit mode. This instruction is ran in 8 bit mode, so Function set must be called again 
  lda #%00100000
  jsr lcd_instruction
  lda #%00101000 ; 4-bit; 2-lines; 5x8
  jsr lcd_instruction
  lda #%00001111 ; Display on; cursor on; blink on
  jsr lcd_instruction
  lda #%00000110 ; Shift cursor
  jsr lcd_instruction
  lda #$00000001 ; Clear display
  jsr lcd_instruction 
  rts

lcd_instruction:
  ; set data direction
  tax ;stash instruction in x
  jsr lcd_busy_wait
  lda #%11111111
  sta DDRA
  sta DDRB
  txa
 

  ;store top of instruction in x
  tay ;stash complete instruction in y
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
  

  ; send top half
  txa
  sta PORTB

  ; pulse enable
  lda V_LCD_SELECT
  sta PORTA
  lda #0
  sta PORTA
  

  ; send bottom half
  tya
  sta PORTB ; set enable line low

  ; pulse enable
  lda V_LCD_SELECT
  sta PORTA
  lda #0
  sta PORTA
  

  rts

lcd_char:
  ; set data direction
  tax ;stash char in x
  jsr lcd_busy_wait
  lda #%11111111
  sta DDRA
  sta DDRB
  txa

  ;store top of char in x
  tay
  and #%11110000
  ora #LCD_RS_MASK
  tax
  tya

  ;store bottom of char in y
  and #%00001111
  asl
  asl
  asl
  asl
  ora #LCD_RS_MASK
  tay
  

  ; send top half
  txa
  sta PORTB

  ; pulse enable
  lda V_LCD_SELECT
  sta PORTA
  lda #0
  sta PORTA
  

  ; send bottom half
  tya
  sta PORTB ; set enable line low

  ; pulse enable
  lda V_LCD_SELECT
  sta PORTA
  lda #0
  sta PORTA

  rts

lcd_busy_wait:
  lda #%11111111
  sta DDRA
  lda #%00001111
  sta DDRB
  
  lda #(LCD_RW_MASK)
  sta PORTB

  ; load top nibble (contains busy flag)
  lda V_LCD_SELECT
  sta PORTA
  lda PORTB
  pha ; push top nibble 
  lda #0
  sta PORTA

  ; load bottom nibble
  lda V_LCD_SELECT
  sta PORTA
  ;ignore it
  lda #0
  sta PORTA

  pla
  and #%10000000
  bne lcd_busy_wait ; restart loop unless every bit is 0
  rts
