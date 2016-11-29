; сохранить регистр статуса
 IN		SREG_INT, SREG
 PUSH	ACCUM
 PUSH	tmp
;------- 
 test1
 
.EQU cint_count_to_timeout = (cms_timeout*cint_freq)/1000 ; Количество прерываний для обеспечения требуемого периода

.IF cint_count_to_timeout > 255 
.error "cint_count_to_timeout constant overflow, get smaller time or bigger int period."
.ENDIF

.IF (cms_timeout*cint_freq) % 1000 != 0 
.warning "cint_count_to_timeout constant is not accurate!"
.endif

; Счет милисекунд, при превышении cms_timeout(400)мс - устанавливаем флаг mstimeout
 inc	stime_cnt
 MOV	ACCUM, stime_cnt
 cpi	ACCUM, cint_count_to_timeout
 brlo	tc_nomstimeout 
 clr	stime_cnt
 set_flag	mstimeout
 
tc_nomstimeout:

; Сканирование клавиатуры и индикатора.
 MOV	ACCUM,	reg0
 ANDI	ACCUM,	~( HG_mask | key_mask ); подготвим к обновлению позиции сканирования
 MOV	reg0,	ACCUM

 inc scan_cnt
; Сначала индикатор 

;--- mov ACCUM, scan_cnt
;--- ANDI ACCUM, 0x01 ; Оставялем первый бит (0 - младший разряд, 1 - старший разряд)
; в зависимости от бита копировать из HG_val0 или HG_val1 в --> reg_seg 
 go_if_set scan_cnt, 0, tc_sel_HG1
 
tc_sel_HG0: 
 mov reg_seg, HG_val0 ; младший разряд
 set_bit reg0, REG_HG0 
 rjmp tc_sel_HGend
 
tc_sel_HG1:
 mov reg_seg, HG_val1 ; старший разряд 
 set_bit reg0, REG_HG1
 
tc_sel_HGend:

; Теперь клавиатура
 mov	ACCUM, scan_cnt
 ANDI	ACCUM, 0b00000111 ; Оставялем биты 0-2 
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

 rcall reg_send ; Обновляем состояние выходов.
 
; если цикл сканирования 0 то пропускаем проверку нажатия
 go_if_clear scan_cnt, 0, tc_no_key_check
   
; читаем вход клавиатуры
 IN		ACCUM, PINB  
 ANDI	ACCUM, (1<<key_in)
 CLC
 BREQ tc_key_not_pres  ; кнопка НЕ нажата
 SEC 
 
tc_key_not_pres:
 ROR keys_stat

tc_no_key_check:


; регистрация нажатий.
 mov	ACCUM, scan_cnt
 ANDI	ACCUM, 0b00000111
 BRNE	tc_no_key_capture
; На этом этапе в keys_stat находится текущее и предыдущее состояние нажатых кнопок 
; требуется в keys_pres отразить событие нажатия
; keys_stat -  биты 0..3 - предыдущее состояние, 4..7 - текущее
; keys_pres

 mov	tmp,	keys_stat
 mov	ACCUM,	keys_stat
; ANDI ACCUM, 0b00001111 ; оставим только предыдущее
 SWAP	tmp			; tmp 0..3 - текущее состояние кнопок, ACCUM 0..3 - предыдущее 
 EOR	ACCUM, tmp	; ACCUM 0..3 = изменение состояния. (1 - изменилось, 0 - осталось прежним)
; COM	tmp			; tmp 0..3 - 1 - отпущено
 AND	ACCUM,	tmp ; Оставим только событие нажатия.
 ANDI	ACCUM, 0b00001111 ; Очистим от лишнего
 OR		keys_pres, ACCUM
tc_no_key_capture:

 test0
;-------
 POP	tmp
 POP	ACCUM
; восстановить регистр статуса?
 OUT	SREG, SREG_INT
