;��ӡ����

proc 1	
	get_player_mapinfo 0 1 2 3 4
	get_point_dist @3 @4 7214 20588 10
	
	task_stat 4024 11 12
	task_stat 4029 13 14
		
		if 12 = 3
			if 14 = 2
				if 2 = 2100
					if 10 <= 500
						CALL_MON 1 233004 0 600 7214 20588 1
						dec_item_num_inpak 5300018 1 0
					else
						add_sys_msg_start
						add_sys_msg ������ָ������Ŷ��
						set_sys_msg_mode 4 0 0
						msg_info
					endif
				else
					add_sys_msg_start
					add_sys_msg Ŀ�겻��ָ����ͼ�У�
					set_sys_msg_mode 4 0 0
					msg_info
				endif
			endif
		else
			add_sys_msg_start
			add_sys_msg ���Ȼظ����ߣ�
			set_sys_msg_mode 4 0 0
			msg_info
		endif
				
			
 

endproc
		