;70����,�����й�����3
PROC 1
	is_pworld_over 1
	if 1 = 1
		return
	endif
	strvar val
        GET_PWORLD_VAR 56 32 1 val
        If val != 0
		return
	endif
	new_mon 205196 1 19800 24700 0 0 0 1 0
	new_mon 205196 1 19600 24700 0 0 0 1 0
	new_mon 205196 1 19300 24700 0 0 0 1 0
	
	add_sys_msg_start
	add_sys_msg ǰ�����ָ߶�Σ���������λע��
	set_sys_msg_mode 3 0 0
	map_msg_info
	;�߼�����road3���,0δ������,1������
	VAR 24 1
	set_pworld_var 56 32 @24 1
ENDPROC
