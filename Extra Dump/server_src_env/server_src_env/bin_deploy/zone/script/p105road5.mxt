;70����,�����й�����5
PROC 1
	is_pworld_over 1
	if 1 = 1
		return
	endif
	strvar val
        GET_PWORLD_VAR 64 32 1 val
        If val != 0
		return
	endif
	new_mon 284069 1 19900 28000 0 0 0 1 0
	new_mon 284070 1 19700 28000 0 0 0 1 0
	new_mon 284070 1 19400 28000 0 0 0 1 0
	;�߼�����road5���,0δ������,1������
	VAR 26 1
	set_pworld_var 64 32 @26 1
	add_sys_msg_start
	add_sys_msg ��η����ʿ��������������Ҫ�Ƚ�����ͨ�����鷳��
	set_sys_msg_mode 3 0 0
	map_msg_info
	add_sys_msg_start
	add_sys_msg ���Ѿ����⸽�����������������и���ϸ����ʾ������ϸѰ�ң�
	set_sys_msg_mode 3 0 0
	map_msg_info
ENDPROC
