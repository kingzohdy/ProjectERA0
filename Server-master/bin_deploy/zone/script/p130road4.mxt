;70����,�����й�����4
PROC 1
	is_pworld_over 1
	if 1 = 1
		return
	endif
	strvar val
        GET_PWORLD_VAR 60 32 1 val
        If val != 0
		return
	endif
	new_mon 205170 1 19900 26700 0 0 0 1 0
	new_mon 205170 1 19600 26700 0 0 0 1 0
	new_mon 205170 1 19500 26700 0 0 0 1 0
	
	add_sys_msg_start
	add_sys_msg �������з���Ԯ������
	set_sys_msg_mode 3 0 0
	map_msg_info
	;�߼�����road4���,0δ������,1������
	VAR 25 1
	set_pworld_var 60 32 @25 1
ENDPROC
