;ս�Ĵ� �߼�����1


PROC 1

	GET_PWORLD_VAR 32 32 1 13
	If 13 = 0
		add_sys_msg_start
		add_sys_msg һֻ����������ս�ġ�������
		set_sys_msg_mode 3 0 0
		map_msg_info
		var 13 1
		set_pworld_var 32 32 @13 1

	endif

ENDPROC