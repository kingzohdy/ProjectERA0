;����  ����ʱ��Ʒ ɾ��Ʒ 

Proc 1
	STRVAR id Ret
	task_stat 1726 1 Ret
	if Ret = 4
		return
	endif
	
		;GET_TARGET_INFO 3 1 id
		get_player_mapinfo 0 1 2 3 4
		get_point_dist @3 @4 14500 29300 100
				if 2 = 2500
					if 100 <= 500
					NEW_OWNER_TMP_ITEM 1460018 14500 29300 0 0 15 0 0 0 0 1
							if @1 = -1
								return
							else 
								;DEC_ITEM_NUM_INPAK 5300480 1 1
								add_sys_msg_start
								add_sys_msg ��������������ƥ�����������������ٶȰ����ﵽ���������ɡ�
								set_sys_msg_mode 3 0 0
								msg_info
							endif
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
endproc			
			
				
				