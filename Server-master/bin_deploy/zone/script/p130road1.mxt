;70����,�����й�����1
PROC 1
	is_pworld_over 1
	if 1 = 1
		return
	endif
	strvar val
        GET_PWORLD_VAR 48 32 1 val
        If val != 0
		return
	endif
	;��ˢ����
;	new_mon 206260 1 19245 21741 0 0 0 1 0 
;	new_mon 206261 1 19245 21741 0 0 0 1 0
;	new_mon 206261 1 19639 21752 0 0 0 1 0
;	PLAYER_MOVIE_START 1 0
	add_sys_msg_start
	add_sys_msg С�ģ����ﴦ��͸¶��Σ�յ���Ϣ 
	set_sys_msg_mode 3 0 0
	map_msg_info
	;�߼�����road1���,0δ������,1������
	VAR 22 1
	set_pworld_var 48 32 @22 1
ENDPROC
