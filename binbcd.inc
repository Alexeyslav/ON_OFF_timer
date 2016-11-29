;	temp1	8-bit binary value
;	temp1	BCD result младший разряд - единицы результата (0..9)
;	temp2	BCD result старший разряд - десятки результата (0..25)

Bin2bcd8:
	clr		temp2		; clear result MSD
bBCD8_1:
	subi	temp1,10	; input = input - 10
	brcs	bBCD8_2		; abort if carry set
	inc		temp2		; inc MSD
	rjmp	bBCD8_1		; loop again
bBCD8_2:
	subi	temp1,-10	; compensate extra subtraction
	ret

	
	