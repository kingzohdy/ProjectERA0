;��ý�ָ����

PROC 1
	var 10 0
	set_task_var 1537 0 8 @10 1

	TASK_STAT 1537 1 20
		
	if 20 = 2 
			add_sys_msg_start
			add_sys_msg ��Ƭ�����ƺ���Щ��ͬ�����ó�Сֽ��������һ�°ɣ�
			set_sys_msg_mode 3 0 0
			msg_info
			
			GET_PLAYER_ID 1 50
			EFFECT_ID 0 @50 playMontion(%d,"9505_16")
	endif

ENDPROC

proc 2
	
	TASK_STAT 1537 1 20
	if 20 = 2 

		get_task_var 1537 0 8 1 10
		add 10 1
		set_task_var 1537 0 8 @10 1
		
		if 10 = 8		
			add_sys_msg_start
			add_sys_msg �ף����������ʲô�������조�ھ�һ�¿�����
			set_sys_msg_mode 3 0 0
			msg_info

		endif 

		if 10 = 10
		;	add_item_num_inpak 5300358 1 1 0
		;	
		;	add_sys_msg_start
		;	add_sys_msg �������ҵ�һ���ƾɵ���Ƥֽ��
		;	set_sys_msg_mode 3 0 0
		;	msg_info
		endif
	
	endif

endproc