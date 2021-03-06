; ��������� ������� �������
 IN		SREG_INT, SREG
 PUSH	ACCUM
 PUSH	tmp
;------- 
 test1
 
.EQU cint_count_to_timeout = (cms_timeout*cint_freq)/1000 ; ���������� ���������� ��� ����������� ���������� �������

.IF cint_count_to_timeout > 255 
.error "cint_count_to_timeout constant overflow, get smaller time or bigger int period."
.ENDIF

.IF (cms_timeout*cint_freq) % 1000 != 0 
.warning "cint_count_to_timeout constant is not accurate!"
.endif

; ���� ����������, ��� ���������� cms_timeout(400)�� - ������������� ���� mstimeout
 inc	stime_cnt
 MOV	ACCUM, stime_cnt
 cpi	ACCUM, cint_count_to_timeout
 brlo	tc_nomstimeout 
 clr	stime_cnt
 set_flag	mstimeout
 
tc_nomstimeout:

; ������������ ���������� � ����������.
 MOV	ACCUM,	reg0
 ANDI	ACCUM,	~( HG_mask | key_mask ); ��������� � ���������� ������� ������������
 MOV	reg0,	ACCUM

 inc scan_cnt
; ������� ��������� 

;--- mov ACCUM, scan_cnt
;--- ANDI ACCUM, 0x01 ; ��������� ������ ��� (0 - ������� ������, 1 - ������� ������)
; � ����������� �� ���� ���������� �� HG_val0 ��� HG_val1 � --> reg_seg 
 go_if_set scan_cnt, 0, tc_sel_HG1
 
tc_sel_HG0: 
 mov reg_seg, HG_val0 ; ������� ������
 set_bit reg0, REG_HG0 
 rjmp tc_sel_HGend
 
tc_sel_HG1:
 mov reg_seg, HG_val1 ; ������� ������ 
 set_bit reg0, REG_HG1
 
tc_sel_HGend:

; ������ ����������
 mov	ACCUM, scan_cnt
 ANDI	ACCUM, 0b00000111 ; ��������� ���� 0-2 
 LSR	ACCUM
  
 SET
tc_is_key0: 
 CPI	ACCUM, 0
 BRNE	tc_is_key1
 BLD	reg0, REG_key0
 
tc_is_key1:
 CPI	ACCUM, 1
 BRNE	tc_is_key2
 BLD	reg0, REG_key1
 
tc_is_key2:
 CPI	ACCUM, 2
 BRNE	tc_is_key3
 BLD	reg0, REG_key2

tc_is_key3:
 CPI	ACCUM, 3
 BRNE	tc_is_key_end
 BLD	reg0, REG_key3 
 
tc_is_key_end:

 rcall reg_send ; ��������� ��������� �������.
 
; ���� ���� ������������ 0 �� ���������� �������� �������
 go_if_clear scan_cnt, 0, tc_no_key_check
   
; ������ ���� ����������
 IN		ACCUM, PINB  
 ANDI	ACCUM, (1<<key_in)
 CLC
 BREQ tc_key_not_pres  ; ������ �� ������
 SEC 
 
tc_key_not_pres:
 ROR keys_stat

tc_no_key_check:


; ����������� �������.
 mov	ACCUM, scan_cnt
 ANDI	ACCUM, 0b00000111
 BRNE	tc_no_key_capture
; �� ���� ����� � keys_stat ��������� ������� � ���������� ��������� ������� ������ 
; ��������� � keys_pres �������� ������� �������
; keys_stat -  ���� 0..3 - ���������� ���������, 4..7 - �������
; keys_pres

 mov	tmp,	keys_stat
 mov	ACCUM,	keys_stat
; ANDI ACCUM, 0b00001111 ; ������� ������ ����������
 SWAP	tmp			; tmp 0..3 - ������� ��������� ������, ACCUM 0..3 - ���������� 
 EOR	ACCUM, tmp	; ACCUM 0..3 = ��������� ���������. (1 - ����������, 0 - �������� �������)
; COM	tmp			; tmp 0..3 - 1 - ��������
 AND	ACCUM,	tmp ; ������� ������ ������� �������.
 ANDI	ACCUM, 0b00001111 ; ������� �� �������
 OR		keys_pres, ACCUM
tc_no_key_capture:

 test0
;-------
 POP	tmp
 POP	ACCUM
; ������������ ������� �������?
 OUT	SREG, SREG_INT
