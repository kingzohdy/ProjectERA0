;70����,�����й�����2
PROC 1
	is_pworld_over 1
	if 1 = 1
		return
	endif
	strvar val
        GET_PWORLD_VAR 52 32 1 val
        If val != 0
		return
	endif
	new_mon 284070 1 19109 22303 0 0 0 1 0
	new_mon 226015 1 19414 22302 0 0 0 1 0
	new_mon 226015 1 19600 22700 0 0 0 1 0
;	new_mon 226015 1 19300 22700 0 0 0 1 0
	
	add_sys_msg_start
	add_sys_msg �ез�Ŀ����֣�ע������
	set_sys_msg_mode 3 0 0
	map_msg_info
	;�߼�����road2���,0δ������,1������
	VAR 23 1
	set_pworld_var 52 32 @23 1
ENDPROC
