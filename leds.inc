; high(led_segments*2) low(led_segments*2)
; input: Accum - number 0..15; output Accum - segments

get_seg_bv:
 LDI Zh, high(led_segments*2)
 LDI Zl, low(led_segments*2)
 ANDI ACCUM, 0x0F
 CLR temp0              ; ���������� ��� 0 � ����������
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
  rcall Bin2bcd8 ; bin->bcd �������������� - temp1 - ������� �����, temp1 - �������, temp2 - �������.
;  �������� ������� ����� �� �����������, ��� ����� ������������ � ��������� �������������� � ��������.
;  mov temp1, temp2 ; ������� ������ � ����� 0..3
;  SWAP temp2       ; ������� ������ � ����� 0..3
  
; ������������� ����� temp2 � temp1 � �������� ��� ������� � ������� ������ 
; ������� �������� ��������� � ������� ��������� HG_Val0, ������� � HG_Val1 ��� ����� ���������� � ����������� �� ���������� ����.
  mov ACCUM, temp1
  rcall get_seg_bv
  mov HG_Val0, ACCUM
  
  mov ACCUM, temp2
  rcall get_seg_bv
  mov HG_Val1, ACCUM
  
  pop temp2
  pop temp1
  
ret