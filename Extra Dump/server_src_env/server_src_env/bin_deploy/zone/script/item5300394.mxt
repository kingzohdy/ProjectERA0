;����Կ��

proc 1


	  get_player_mapinfo 0 1 2 3 4

	  if 2 = 2100 or 2 = 2101 or 2 = 2102 or 2 = 2103 or 2 = 2105 or 2 = 2106 or 2 = 2107 or 2 = 2108

;			dec_item_num_inpak 5300394 1 0
;			ADD_ITEM_NUM_INPAK 5300289 1 1 0

			GET_MON_INFO 0 0 0 0 0 0 20 21

			NEW_LIFE_NPC 0 313000 8834 17779 120 30 tgjisi.mac
			add_sys_msg_start
			add_sys_msg ʱ�����ޣ���û�ѯ��һЩ�м�ֵ�Ķ����ɣ�
			set_sys_msg_mode 4 0 0
			msg_info
			DESTROY_MON 10				
			
;			get_task_var 1323 0 8 1 10
;			var 10 1
;			set_task_var 1323 0 8 @10 1

	  else
			add_sys_msg_start
			add_sys_msg Ŀ�겻�ڱ���ͼ�У�
			set_sys_msg_mode 3 0 0
			msg_info

	  endif

endproc 

