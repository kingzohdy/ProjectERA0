;ˮ����


proc 1
	  get_player_mapinfo 0 1 2 3 4

	  get_point_dist @3 @4 17964 13305 10
	  if 2 = 2200
		if 10 <= 500  
			
			dec_item_num_inpak 5300335 1 0
			ADD_ITEM_NUM_INPAK 5300336 1 1 0
			add_sys_msg_start
			add_sys_msg �ҵ�ʯͷ����򿪱�����������Ʒ����ʹ��װ��ˮ��ˮ����
			set_sys_msg_mode 3 0 0
			msg_info

		else
			add_sys_msg_start
			add_sys_msg ���ȵ���ѩԭȡˮ�㣬��ʹ��ˮ��װˮ��
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


