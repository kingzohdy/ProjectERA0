;������� ,��ǲ��1
;�����¼�
proc 2
	strvar x y
	GET_MON_INFO 1 0 0 0 0 0 x y
	;��ʱ��е,��ӿ�
	;new_tmp_machine ::tempmachine1::
	NEW_MON_MACHINE 1 291000 0 1 @x @y 0
	;��е1����λ��x
	VAR 21 @x
	set_pworld_var 44 32 @21 1
	;��е1����λ��y
	VAR 22 @y
	set_pworld_var 48 32 @22 1
	;��е1ˢ�±��,0δ������,1������
	VAR 18 1
	set_pworld_var 32 32 @18 1
endproc
