/*  
 * Created: 28.09.2015
 *   Author: Alexey
 */ 
.include "tn13def.inc"
; тактовая частота - 4.8Мгц
; Fuses -	CKSEL0=1, CKSEL1=0, {4.8Mhz internal}
;			SUT0=0, SUT1=1, {14CK + 64 ms}
;			CKDIV8=1, 
;			WDTON=1, EESAVE=1, RSTDISBL=1, {не трогать, оно не нужно}
;			BODLEVEL0=1, BODLEVEL1=1, {BOD disabled}
;			SPMEN=1
; 0 - programmed, 1 - unprogrammed.

; Константы
;.EQU const  = 10
.EQU CPU_Freq = 4800000

; Общие регистры.

#define true 1
#define false 0

;#define DBG blabla
;.SET DBG = true

; Определения

;Макросы
.include "macros.inc"
.include "app_config.inc"


;.macro EEPROM_read ; <адрес> <регистр>
; OUT             EEAR, @0  ; Адрес ЕЕПРОМ
; SBI             EECR, 0              ; Команда чтения
; IN              @1, EEDR             ; Значение читаем куда нам нужно
;.endmacro


; Регистровые переменные
.DEF temp0		= R0
.DEF reg0		= R1 ; отображение внешнего вспомогательного регистра (выходы сканирвоания индикатора, клавиатуры и др.)
.DEF reg_seg	= R2 ; отображение внешнего регистра сегментов индикатора.
.DEF scan_cnt	= R3 ; Счетчик для сканирования сегментов(бит 0), клавиатуры(биты 1 и 2)
.DEF stime_cnt	= R4 ; Счетчик миллисекунд времени.
.DEF Flags		= R5 ; регистр флагов состояния программы
.EQU edit_mode = 0   ; программа в режиме настройки!!!
.EQU mstimeout = 1   ; 400ms time is out! 
.EQU ONstate   = 2   ; Подать сигнал на включение!
.EQU mtimeout  = 3	 ; 1min time is out!
;.EQU           = 4 ;
;.EQU           = 5 ;
;.EQU           = 6 ;
;.EQU           = 7 ;

.DEF HG_val0	= R6 ; младший разряд индикатора
.DEF HG_val1	= R7 ; старший разряд индикатора
.DEF keys_stat	= R8 ; биты 0..3 - предыдущее состояние, 4..7 - текущее
.DEF keys_pres	= R9 ; биты 0..3 - было нажатие кнопки(однократное)
.DEF mtime_count= R10 ; Счётчик событий mstimeout, 150 = 1 минуте
.DEF tmp  		= R11
.DEF SREG_INT	= R12 ; используется в прерывании для быстрого сохранения регистра статуса(без использования стека)

; Счетчики в регистрах
.DEF cyclecount = R16
.DEF loopscount = R17
.DEF tmp2       = R18 ; с возможностью непосредственной загрузки значения через LDI
.DEF temp1      = R18 ; Для использования внутри процедур, восстановить значение перед выходом
.DEF temp2      = R19 ; Для использования внутри процедур, восстановить значение перед выходом
.DEF loopscount2= R19
.DEF cur_time	= R20 ; Счетчик текущего времени


.DEF ACCUM      = R25

; ======= Ячейки памяти  =========== RAM начинается с адреса 0x60-0x9F
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
 ; Начальное значение на выходах контроллера
 set_io PORTB, (default_clk_pin|default_data_pin|default_lock_pin|default_relay_pin)
 
 ; установить порты ввода-вывода
 set_io DDRB,   0b00010111  ; 1 - выход, 0 - вход.
 
; настройка таймера
; Цель - частота прерываний ~400гц, необходим делитель = 12000. прескалер ~ 47.
; прескалер выставить на 64, таймер считает до OCRA = 186. mode = 2, прерывание от таймера - разрешить.
; Частота прерываний 4800000/(64 * 187) = 401.1 Гц.
; Если взять делитель 150 получится частота ровно 500 Гц
; Если взять делитель 250 получится частота ровно 300 Гц

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
	
 set_io TCCR0A, (0b10 << WGM00)	; режим таймера 02 - CTC. [WGM = 0b10]
 set_io TCCR0B, (0b011 << CS00)	; Счетчик работает c предделителем = 64.
 set_io TIMSK0, (1 << OCIE0A)   ; разрешить прерывание по сравнению OCR0A
 set_io OCR0A , timer_int_divizor
 SEI

; Собственно сам цикл программы
;========================================================================================================
;
;========================================================================================================

START:


go_if_clear flags, mstimeout, ml_nomstick
; Сбрасываем флаг
 clear_bit flags, mstimeout
 ; Посчитаем количество квантов до cmin_timeout и дёргаем счетчик минут.

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

; ======== Ячейки EEPROM ========
;.ESEG
;programm:
