;��ˮ��ƿ


proc 1
	  get_player_mapinfo 0 1 2 3 4

	  get_point_dist @3 @4 13329 2892 10
	  if 2 = 2200
			GET_PLAYER_ID 1 54
			GET_ID_PLAYER_STATUS_INFO @54 1099 60 3 0 0 0
			if 60 = 0
				dec_item_num_inpak 5300330 1 0
				ADD_ITEM_NUM_INPAK 5300298 1 1 0
				
				GET_MON_INFO 10 11 12 13 14 15 16 17 
				effect 0 0 playEffct($@16$,$@17$,"effect\\entity\\9547_16.ent")	
				del_mon 1
			else
				add_sys_msg_start
				add_sys_msg ʡ�������ɣ���ƣ���������ŷ�����ˣ�����ж�ʧ���ˣ�
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


