; key_plus_mask
; key_minus_mask
; key_set_mask
; key_na_mask

.macro is_nkeypress ;
; @0 - ����� �������(<key_xxx_mask>) � �������� keys_pres, ��������� ���������� � app_config.inc ������ [03-01]
; @1 - ����� �������� ���� ������� �� ������, ����� ����������� ��������� ����� ������� ����������

 go_if_set keys_pres, @0, ikn_keyispres
 rjmp @1
ikn_keyispres:
 clear_bit keys_pres, @0 ; ���������� ������� ������� �������������� ������
.endmacro