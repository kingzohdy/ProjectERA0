;����ˮ��

proc 1
	  get_player_mapinfo 0 1 2 3 4
	  get_point_dist @3 @4 5100 4900 10
	  if 2 = 2300
		if 10 <= 500
			
;			dec_item_num_inpak 5300375 1 0
;			ADD_ITEM_NUM_INPAK 5300289 1 1 0
			CALL_MON 1 293054 1000 60 5100 4900 0
			
		else
			add_sys_msg_start
			add_sys_msg ����ָ������
			set_sys_msg_mode 3 0 0
			msg_info
		endif

	  else
			add_sys_msg_start
			add_sys_msg Ŀ�겻�ڱ���ͼ�У�
			set_sys_msg_mode 3 0 0
			msg_info
	  endif
	
endproc
