;�ٻ�����

proc 1

	task_stat 1746 1 40
	if 40 = 4
			add_sys_msg_start
			add_sys_msg �����Ѿ�ʧ�ܣ��������������½�ȡ��
			set_sys_msg_mode 1 0 0
			msg_info
		return
	endif


	STRVAR id a m playerid
	get_task_var 1746 2 32 1 id
	get_task_var 1746 1 8 1 a
	;GET_PLAYER_ID 1 playerid
	GET_MAP_MON_NUM 2500 235012 1 m 1 
	if m > 0
			add_sys_msg_start
			add_sys_msg �Ѿ������ڴ��ٻ������ҵ�������ٻ���ʽ�ɡ�
			set_sys_msg_mode 3 0 0
			msg_info	
	else
		get_player_mapinfo 0 1 2 3 4
		get_point_dist @3 @4 20300 33100 100
			if 2 = 2500
				if 100 <= 500	
					effect 2 0 startGray()
					;TARGET_ADD_STATUS 1 @playerid 1018 1 120 1
					CALL_MON 1 235012 0 120 20400 33200 id
					set_task_var 1746 2 32 @id 1
						if 1 = -1
							return
						else
							DEC_ITEM_NUM_INPAK 5300497 1 1
							add_sys_msg_start
							add_sys_msg ��������ˣ���ȥ�������񣬾��Բ�������ʽ����ϣ�
							set_sys_msg_mode 3 0 0
							msg_info
						endif
					get_task_var 1746 1 8 1 a
						var a 2
					set_task_var 1746 1 8 @a 1
				else
					add_sys_msg_start
					add_sys_msg ����ָ������
					set_sys_msg_mode 3 0 0
					msg_info
				endif
			else
				add_sys_msg_start
				add_sys_msg �������ڴ˵�ͼʹ�ã�
				set_sys_msg_mode 3 0 0
				msg_info
			endif
	endif




endproc
