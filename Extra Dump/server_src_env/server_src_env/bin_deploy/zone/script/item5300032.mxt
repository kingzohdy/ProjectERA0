;��ӡ������ƿ��
proc 1
	get_player_mapinfo 0 1 2 3 4
	get_point_dist @3 @4 15517 7816 10
	if 2 = 2100
		if 10 <= 500  
		CALL_MON 1 231022 100 600 15517 7816 1
		dec_item_num_inpak 5300032 1 0  
		add_sys_msg_start
		add_sys_msg ���ɱ����ʳ��֮�꣬�������÷�ӡ��
		set_sys_msg_mode 4 0 0
		msg_info
		else
			add_sys_msg_start
			add_sys_msg ������ָ������Ŷ��
			set_sys_msg_mode 4 0 0
			msg_info
		endif
	  else
			add_sys_msg_start
			add_sys_msg Ŀ�겻�ڱ���ͼ�У�
			set_sys_msg_mode 4 0 0
			msg_info
	  endif
	
endproc

		