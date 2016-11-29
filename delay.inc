.EQU one_step_delay_ms  = 10 ; ƒлительность одного шага дл€ переходов €ркости в милисекундах
.EQU CPU_freq = 4800 ; “актова€ частота, килогерц

delay_step:  // задержка дл€ одного шага в процедуре изменени€ €ркости.
					; рассчитаем примерное количество тактов на 10мс -
					; N = 0.01c*4800000√ц = 48000
.EQU ds_inner_cycles = 256*4
.EQU ds_lpsc = (one_step_delay_ms * CPU_freq) / ds_inner_cycles

LDI		loopscount, ds_lpsc ; внешний цикл. 46*1024
l_loop1:
LDI		loopscount2, $00  ; 256 итераций. 1024 такта
l_loop2:
NOP					; 1*N
DEC 	loopscount2	; 1*N
BRNE	l_loop2     ; 2*N
DEC 	loopscount
BRNE	l_loop1

RET

delay_01s:
.EQU d01s_loops = 100 / one_step_delay_ms
 LDI loopscount, d01s_loops

d01s_loop:
  push	loopscount
  rcall delay_step
  pop	loopscount
  DEC 	loopscount
 BRNE	d01s_loop
RET

delay_1s:
.EQU d1s_loops = 1000 / one_step_delay_ms
 LDI loopscount, d1s_loops

d1s_loop:
  push loopscount
  rcall delay_step
  pop loopscount
  DEC 	loopscount
 BRNE	d1s_loop
RET
