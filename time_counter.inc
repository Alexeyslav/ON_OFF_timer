; ��������� ����� � ����������� �������
; +1 ������, ������� �������� ������������(���� ��������?)
time_nextmin:
  inc	cur_time
  CPI	cur_time, 100
  CLC
  BRLT	tnm_nogreat 
; ���� ������ - �������� � ��������� ������� ��������  
  CLR	cur_time
  SEC
tnm_nogreat:  
ret



; -1 ������, ������� �������� ������������(���� ��������?)
time_prevmin:
  dec	cur_time
;  ���� ������� - ��������� �������� 99
  BRCC	tnm_nooverflow
  LDI	cur_time, 99
tnm_nooverflow:
ret
; ���������� ������� ����� �� ����������.

display_cur_time:
 push temp1
 push temp2
	mov temp1, cur_time
	rcall BIN2BCD8       ; ����� �� ��� ������������ - temp1 �������, temp2 - �������(���� ����� �� ������ 100)
	mov accum, temp1
	rcall get_seg_bv
    mov HG_val1, accum
	mov accum, temp2
	rcall get_seg_bv
	mov HG_val0, accum
 pop temp2
 pop temp1
ret 

time_display:
 PUSH	ACCUM
  mov	ACCUM,	cur_time
  rcall	value_display
 POP	ACCUM
ret
