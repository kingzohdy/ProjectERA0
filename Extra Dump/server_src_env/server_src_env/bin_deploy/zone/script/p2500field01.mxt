;�������֮��ʲô�� 1744�����

PROC 1
	var 10 0
	set_task_var 1744 0 8 @10 1

	TASK_STAT 1744 1 20
		
	if 20 = 2 
			add_sys_msg_start
			add_sys_msg ��Ƭ�����ƺ���Щ��ͬ����ϸ̽��һ�°ɣ�
			set_sys_msg_mode 3 0 0
			msg_info
	endif

ENDPROC

proc 2
	
	TASK_STAT 1744 1 20
	if 20 = 2 

		get_task_var 1744 0 8 1 10
		add 10 1
		set_task_var 1744 0 8 @10 1
		if 10 = 8		
			add_sys_msg_start
			add_sys_msg �ף����������ʲô������
			set_sys_msg_mode 3 0 0
			msg_info
			
		endif 
		
		if 10 = 10
			add_item_num_inpak 5300495 1 1 0
			
			strvar playerid
			effect 2 0 stopGray()
			GET_PLAYER_ID 1 playerid
			TARGET_ADD_STATUS 1 @playerid 1018 1 0 1
			add_sys_msg_start
			add_sys_msg ������һ������֮�顣
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	
	endif

endproc
