;������� ,��ǲ��2
;�����¼�
proc 2
	strvar x y
	GET_MON_INFO 1 0 0 0 0 0 x y
	;��ʱ��е,��ӿ�
	;new_tmp_machine ::tempmachine2::
	NEW_MON_MACHINE 1 291001 0 1 @x @y 0
	;��е2����λ��x
	VAR 23 @x
	set_pworld_var 52 32 @23 1
	;��е2����λ��y
	VAR 24 @y
	set_pworld_var 56 32 @24 1
	;��е2ˢ�±��,0δ������,1������
	VAR 19 1
	set_pworld_var 36 32 @19 1
endproc
