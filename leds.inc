; high(led_segments*2) low(led_segments*2)
; input: Accum - number 0..15; output Accum - segments

get_seg_bv:
 LDI Zh, high(led_segments*2)
 LDI Zl, low(led_segments*2)
 ANDI ACCUM, 0x0F
 CLR temp0              ; используем как 0 в арифметике
 ADD Zl, ACCUM
 ADC Zh, temp0
 LPM ACCUM, Z
.if inverse_led_segment == 1
 COM ACCUM
.endif 
 
ret

value_display:
  push	temp1
  push	temp2
  
  mov	temp1,	ACCUM
  rcall Bin2bcd8 ; bin->bcd преобразование - temp1 - входное число, temp1 - единицы, temp2 - десятки.
;  Обрезать старший ниббл не обязательно, это будет осуществлено в процедуре преобразования в сегменты.
;  mov temp1, temp2 ; Младший разряд в битах 0..3
;  SWAP temp2       ; Старший разряд в битах 0..3
  
; преобразовать цифры temp2 и temp1 в сегменты для младшей и старшей частей 
; младшее значение загрузить в регистр сегментов HG_Val0, старшее в HG_Val1 оно потом подцепится и отобразится на индикаторе само.
  mov ACCUM, temp1
  rcall get_seg_bv
  mov HG_Val0, ACCUM
  
  mov ACCUM, temp2
  rcall get_seg_bv
  mov HG_Val1, ACCUM
  
  pop temp2
  pop temp1
  
ret