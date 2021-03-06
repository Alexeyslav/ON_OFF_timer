; ������������ �����������, ��������� �� ����� ������������� �����������.
; [01-01]
; ������ �����������
#define data_pin	PB0
#define lock_pin	PB1
#define clk_pin 	PB2
#define key_in		PB3
#define relay_pin	PB4
#define test_pin	PB4

; [01-02]
; DEBUG  
#define test1	SBI PORTB, relay_pin
#define test0	CBI PORTB, relay_pin

; [02-01]
; ���������� �������� ��� ��������� (TB62725FN)
#define HG_SEG_a	6
#define HG_SEG_b	7
#define HG_SEG_c	2
#define HG_SEG_d	3
#define HG_SEG_e	4
#define HG_SEG_f	5
#define HG_SEG_g	0
#define HG_led		1

#define inverse_led_segment 1  ; ���.0 ����������� �������

; [02-02]
; ���������� ���������������� �������� HC595
#define REG_HG1		0  ; ������� ������ ����������
#define REG_HG0		1  ; ������� ������ ����������
#define REG_sysled	2
#define REG_res2	3
#define REG_key0	4
#define REG_key1	5
#define REG_key2	6
#define REG_key3	7

; [03-01]
; ������������ ���������� ������ => ���������� ������
#define key_plus	0  ; ����������� ����� ���.������ ���������� 
#define key_minus	1  ; ����������� ��������� � ������ ����� ����������� � [06-01]
#define key_set		2
#define key_na		3 

; ����������� ���������
; [04-01]
; ��������� �������� ������� ����������� � ���� �����.
.EQU default_clk_pin   = (0 << clk_pin  )
.EQU default_data_pin  = (0 << data_pin )
.EQU default_lock_pin  = (0 << lock_pin )
.EQU default_relay_pin = (0 << relay_pin)

; [05-01]
.EQU HG0_mask = (1<<REG_HG0)
.EQU HG1_mask = (1<<REG_HG1)
.EQU HG_mask  = HG0_mask | HG1_mask

; [05-02]
.EQU key0_mask = (1<<REG_key0)
.EQU key1_mask = (1<<REG_key1)
.EQU key2_mask = (1<<REG_key2)
.EQU key3_mask = (1<<REG_key3)
.EQU key_mask  = key0_mask | key1_mask | key2_mask | key3_mask

; [06-01]
.EQU key_plus_mask	= (1<< key_plus)
.EQU key_minus_mask	= (1<< key_minus)
.EQU key_set_mask	= (1<< key_set)
.EQU key_na_mask	= (1<< key_na)

; [07]
.EQU cint_freq		= 500 ; ������� ����������
.EQU cms_timeout	= 400 ; ����� ������� ��� ���������
.EQU cmin_timeout	= 60000 / cms_timeout ; ���������� ������� ������� ��� ����� ������ (60000/400 = 150)