;������Ԫ�غ���  ѡ��ʹ��

proc 1

	
	  get_player_mapinfo 0 1 2 3 4
	  get_point_dist @3 @4 3300 10200 10
	  if 2 = 2300
		if 10 <= 500
			dec_item_num_inpak 5300376 1 0
;			ADD_ITEM_NUM_INPAK 5300289 1 1 0
			strvar PlyMid		

			get_task_var 1532 4 8 1 20
			var 20 1
			set_task_var 1532 4 8 @20 1
			GET_PLAYER_ID 1 PlyMid 
			
;			GET_MON_INFO 0 30 31 32 33 34 35 36		
			del_mon 1		
			strvar MonMid Num
			CALL_MON 1 226010 500 60 3300 10200 MonMid
			
			add_sys_msg_start
			add_sys_msg ���ݶ�˹�����Ļ�Ӱ����ֹ�������Ͽ���������
			set_sys_msg_mode 3 0 0
			msg_info
			
			SET_MON_CURR @MonMid 1
			set_mon_var 0 32 @PlyMid 1
		else
			add_sys_msg_start
			add_sys_msg ��Ҫ��ָ������ſ�ʹ�ã�
			set_sys_msg_mode 3 0 0
			msg_info
		endif
		
	
	  else
			add_sys_msg_start
			add_sys_msg Ŀ�겻�ڱ���ͼ�У�
			set_sys_msg_mode 4 0 0
			msg_info
	  endif

endproc
