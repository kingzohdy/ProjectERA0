;70����,���boss,������������
proc 1
	is_pworld_over 1
	if 1 = 1
		return
	endif
	;boss1�����׶�
	get_pworld_var 80 32 1 30
	if 30 > 0
		return
	endif
	var 30 1
	set_pworld_var 80 32 @30 1
	;boss1flag32,boss1�ٻ����,0δ����,1�Ժ�Ϊ��ʱ
	var 32 1
	set_pworld_var 88 32 @32 1
	add_sys_msg_start
	add_sys_msg ��������֣�������������
	set_sys_msg_mode 1 0 0
	map_msg_info
endproc
