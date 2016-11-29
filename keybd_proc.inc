; key_plus_mask
; key_minus_mask
; key_set_mask
; key_na_mask

.macro is_nkeypress ;
; @0 - маска клавиши(<key_xxx_mask>) в регистре keys_pres, константы определены в app_config.inc раздел [03-01]
; @1 - метка перехода если клавиша НЕ НАЖАТА, иначе выполняется следующая после макроса инструкция

 go_if_set keys_pres, @0, ikn_keyispres
 rjmp @1
ikn_keyispres:
 clear_bit keys_pres, @0 ; сбрасываем признак нажатия соотвествующей кнопки
.endmacro