;����

proc 1

	  get_player_mapinfo 0 1 2 3 4

	  if 2 = 2100 or 2 = 2101 or 2 = 2102 or 2 = 2103 or 2 = 2105 or 2 = 2106 or 2 = 2107 or 2 = 2108

				;	get_player_id 0 20
				;	effect_id 0 @20 playMontion(%d,"9565_16")
;			dec_item_num_inpak 5300415 1 0
		strvar Mid
		GET_MAP_MON_ID 293087 Mid 1
		
		get_task_var 1318 0 8 1 10
		add 10 1
		set_task_var 1318 0 8 @10 1
		if 10 = 1
			mon_speech @Mid 2 0 1 ���Ǵ���Ҳ����͸¶�κ�ѶϢ����ģ�
		endif
		if 10 = 2
		
			mon_speech @Mid 2 0 1 �ñ�̢���ĸ�����Щ�ɣ�
		endif
		if 10 = 3
		
			mon_speech @Mid 2 0 1 ����������ƶ�������ƣ����䲻������
		endif
		if 10 = 4
		
			mon_speech @Mid 2 0 1 ����֪���ɰ��������𣿵���������ȥ�ɣ�
		endif
		if 10 = 5
		
			mon_speech @Mid 2 0 1 ������Ҵ�������Ҳ�������ɰͣ�
			task_fini 1318 0
		endif
		if 10 > 5
			strvar Rad
			rand Rad 2
			if Rad = 0
				mon_speech @Mid 2 0 1 �������鰡�����Ѿ���֪���Ķ��������ˣ�
			
			endif
			if Rad = 1
				mon_speech @Mid 2 0 1 �Ź��Ұɣ���֪���Ķ�˵�ˣ�������ٴ��ˣ�
				
			endif
			
		var 10 4 
		set_task_var 1318 0 8 @10 1
		endif
				
				
;		else
;			add_sys_msg_start
;			add_sys_msg ����ûЧ��������������˵����ʹ�����ԣ�
;			set_sys_msg_mode 3 0 0
;			msg_info
;			
;		endif
	  else
			add_sys_msg_start
			add_sys_msg Ŀ�겻�ڱ���ͼ�У�
			set_sys_msg_mode 3 0 0
			msg_info
	  endif

endproc 

