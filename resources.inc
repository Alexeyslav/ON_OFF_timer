led_segments:
; 0, 1
.db (1<<HG_SEG_a)|(1<<HG_SEG_b)|(1<<HG_SEG_c)|(1<<HG_SEG_d)|(1<<HG_SEG_e)|(1<<HG_SEG_f), 				(1<<HG_SEG_b)|(1<<HG_SEG_c)
; 2, 3
.db (1<<HG_SEG_a)|(1<<HG_SEG_b)|(1<<HG_SEG_g)|(1<<HG_SEG_e)|(1<<HG_SEG_d), 				 				(1<<HG_SEG_a)|(1<<HG_SEG_b)|(1<<HG_SEG_c)|(1<<HG_SEG_d)|(1<<HG_SEG_g)
; 4, 5
.db (1<<HG_SEG_f)|(1<<HG_SEG_g)|(1<<HG_SEG_b)|(1<<HG_SEG_c),			 				 				(1<<HG_SEG_a)|(1<<HG_SEG_f)|(1<<HG_SEG_g)|(1<<HG_SEG_c)|(1<<HG_SEG_d)
; 6, 7
.db (1<<HG_SEG_a)|(1<<HG_SEG_f)|(1<<HG_SEG_e)|(1<<HG_SEG_d)|(1<<HG_SEG_c)|(1<<HG_SEG_g),				(1<<HG_SEG_a)|(1<<HG_SEG_b)|(1<<HG_SEG_c)
; 8, 9
.db (1<<HG_SEG_a)|(1<<HG_SEG_b)|(1<<HG_SEG_c)|(1<<HG_SEG_d)|(1<<HG_SEG_e)|(1<<HG_SEG_f)|(1<<HG_SEG_g),	(1<<HG_SEG_a)|(1<<HG_SEG_b)|(1<<HG_SEG_c)|(1<<HG_SEG_d)|(1<<HG_SEG_f)|(1<<HG_SEG_g)
; -,_
.db (1<<HG_SEG_g), (1<<HG_SEG_d)
; E,
.db (1<<HG_SEG_a)|(1<<HG_SEG_d)|(1<<HG_SEG_e)|(1<<HG_SEG_f)|(1<<HG_SEG_g),	0
;  ,
.db 0, 0
