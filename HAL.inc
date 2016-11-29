; H.A.L of ON-OFF timer project.

; shift register procedures (soft-SPI)
; reg0 - внешний вспомогательный регистр 
; reg1 - внешний вспомогательный регистр сегментов индикатора
; первым в регистр идёт 7-й бит reg1

#define set_data_pin SBI PORTB, data_pin
#define clr_data_pin CBI PORTB, data_pin

#define set_clk_pin SBI PORTB, clk_pin
#define clr_clk_pin CBI PORTB, clk_pin

#define set_lock_pin SBI PORTB, lock_pin
#define clr_lock_pin CBI PORTB, lock_pin


.macro reg_do_clk
  set_clk_pin
  NOP
  clr_clk_pin
.endmacro

.macro reg_do_lock
  set_lock_pin
  NOP
  clr_lock_pin
.endmacro

reg_send_byte: ; передача по SPI, старшим вперёд
 
 LDI	cyclecount, 8
 
rsb_loop:
 ROL	ACCUM

 BRCS	rsb_dt_is1
rsb_dt_is0:
 clr_data_pin
 rjmp	rsb_dt_end
 
rsb_dt_is1:
 set_data_pin
 
rsb_dt_end:

 reg_do_clk
 
 dec	cyclecount
 BRNE	rsb_loop
 
 clr_data_pin
 
RET

reg_send:
 push	cyclecount
 
 mov	ACCUM, reg_seg
 rcall	reg_send_byte
 
 mov	ACCUM, reg0
 rcall	reg_send_byte
   
 reg_do_lock
 
 pop	cyclecount
RET

; Relay control 
#define ON_relay	SBI PORTB, relay_pin
#define OFF_relay	CBI PORTB, relay_pin
