/*  
 * Created: 28.09.2015
 *   Author: Alexey
 */ 
.include "tn13def.inc"
; �������� ������� - 4.8���
; Fuses -	CKSEL0=1, CKSEL1=0, {4.8Mhz internal}
;			SUT0=0, SUT1=1, {14CK + 64 ms}
;			CKDIV8=1, 
;			WDTON=1, EESAVE=1, RSTDISBL=1, {�� �������, ��� �� �����}
;			BODLEVEL0=1, BODLEVEL1=1, {BOD disabled}
;			SPMEN=1
; 0 - programmed, 1 - unprogrammed.

; ���������
;.EQU const  = 10
.EQU CPU_Freq = 4800000

; ����� ��������.

#define true 1
#define false 0

;#define DBG blabla
;.SET DBG = true

; �����������

;�������
.include "macros.inc"
.include "app_config.inc"


;.macro EEPROM_read ; <�����> <�������>
; OUT             EEAR, @0  ; ����� ������
; SBI             EECR, 0              ; ������� ������
; IN              @1, EEDR             ; �������� ������ ���� ��� �����
;.endmacro


; ����������� ����������
.DEF temp0		= R0
.DEF reg0		= R1 ; ����������� �������� ���������������� �������� (������ ������������ ����������, ���������� � ��.)
.DEF reg_seg	= R2 ; ����������� �������� �������� ��������� ����������.
.DEF scan_cnt	= R3 ; ������� ��� ������������ ���������(��� 0), ����������(���� 1 � 2)
.DEF stime_cnt	= R4 ; ������� ����������� �������.
.DEF Flags		= R5 ; ������� ������ ��������� ���������
.EQU edit_mode = 0   ; ��������� � ������ ���������!!!
.EQU mstimeout = 1   ; 400ms time is out! 
.EQU ONstate   = 2   ; ������ ������ �� ���������!
.EQU mtimeout  = 3	 ; 1min time is out!
;.EQU           = 4 ;
;.EQU           = 5 ;
;.EQU           = 6 ;
;.EQU           = 7 ;

.DEF HG_val0	= R6 ; ������� ������ ����������
.DEF HG_val1	= R7 ; ������� ������ ����������
.DEF keys_stat	= R8 ; ���� 0..3 - ���������� ���������, 4..7 - �������
.DEF keys_pres	= R9 ; ���� 0..3 - ���� ������� ������(�����������)
.DEF mtime_count= R10 ; ������� ������� mstimeout, 150 = 1 ������
.DEF tmp  		= R11
.DEF SREG_INT	= R12 ; ������������ � ���������� ��� �������� ���������� �������� �������(��� ������������� �����)

; �������� � ���������
.DEF cyclecount = R16
.DEF loopscount = R17
.DEF tmp2       = R18 ; � ������������ ���������������� �������� �������� ����� LDI
.DEF temp1      = R18 ; ��� ������������� ������ ��������, ������������ �������� ����� �������
.DEF temp2      = R19 ; ��� ������������� ������ ��������, ������������ �������� ����� �������
.DEF loopscount2= R19
.DEF cur_time	= R20 ; ������� �������� �������


.DEF ACCUM      = R25

; ======= ������ ������  =========== RAM ���������� � ������ 0x60-0x9F
.DSEG

.CSEG
.ORG 0
 rjmp RESET ; Reset Handler
 
.ORG OC0Aaddr
 rjmp TIM0_COMPA ; Timer0 CompareA Handler
 
 

;==============  INTERRUPT FROM TIMER
TIM0_COMPA:

.include "timer_int.inc"

RETI
;==============  END OF INTERRUPT

;=====================================================
;                PROGRAM BEGIN THERE!!!
;=====================================================
.include "HAL.inc"
;.include "delay.inc"

RESET:
 set_io SPL, low(RAMEND)
 ; ��������� �������� �� ������� �����������
 set_io PORTB, (default_clk_pin|default_data_pin|default_lock_pin|default_relay_pin)
 
 ; ���������� ����� �����-������
 set_io DDRB,   0b00010111  ; 1 - �����, 0 - ����.
 
; ��������� �������
; ���� - ������� ���������� ~400��, ��������� �������� = 12000. ��������� ~ 47.
; ��������� ��������� �� 64, ������ ������� �� OCRA = 186. mode = 2, ���������� �� ������� - ���������.
; ������� ���������� 4800000/(64 * 187) = 401.1 ��.
; ���� ����� �������� 150 ��������� ������� ����� 500 ��
; ���� ����� �������� 250 ��������� ������� ����� 300 ��

.EQU timer_pre_divisor = 64
.EQU timer_int_divizor = CPU_Freq/(cint_freq * timer_pre_divisor)

.if timer_int_divizor > 255 
.error "Timer divisor is overflow"
.elseif timer_int_divizor < 10
.error "Timer divisor is underflow"
.endif

.IF CPU_Freq % (cint_freq * timer_pre_divisor) != 0 
.warning "Timer divisor is not accurate!"
.endif
	
 set_io TCCR0A, (0b10 << WGM00)	; ����� ������� 02 - CTC. [WGM = 0b10]
 set_io TCCR0B, (0b011 << CS00)	; ������� �������� c ������������� = 64.
 set_io TIMSK0, (1 << OCIE0A)   ; ��������� ���������� �� ��������� OCR0A
 set_io OCR0A , timer_int_divizor
 SEI

; ���������� ��� ���� ���������
;========================================================================================================
;
;========================================================================================================

START:


go_if_clear flags, mstimeout, ml_nomstick
; ���������� ����
 clear_bit flags, mstimeout
 ; ��������� ���������� ������� �� cmin_timeout � ������ ������� �����.

mov	HG_val0,	keys_pres
COM	HG_val0

inc		cur_time
CPI		cur_time, 100
BRLO ct_no_over
 LDI cur_time, 0
ct_no_over:
rcall time_display

clr		keys_pres

ml_nomstick:

RJMP start
;========================================================================================================
.include "leds.inc"
.include "binbcd.inc"
.include "time_counter.inc"
.include "keybd_proc.inc"
.include "resources.inc"


.IFDEF DBG
.warning "DEBUG MODE SOURCE compiled!"
.ENDIF

; ======== ������ EEPROM ========
;.ESEG
;programm:
