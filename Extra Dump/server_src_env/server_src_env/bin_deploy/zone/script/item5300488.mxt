;��¹����  �Թ���ʹ�ã���� ��BUFF ������� ɾ��Ʒ


Proc 1
	
	STRVAR id1 id2 Ret
	task_stat 1739 1 Ret
	if Ret = 2
	
		GET_TARGET_INFO 3 1 id1
		GET_PLAYER_ID 1 id2
		;GET_TARGET_INFO 1 1 id2
			DESTROY_MON 1
			if 1 = -1
				return
			else
				TARGET_ADD_STATUS 1 @id2 1219 2 600 1

				DEC_ITEM_NUM_INPAK 5300488 1 1
				
				strvar m1
				get_task_var 1739 0 8 1 m1
				var m1 1
				set_task_var 1739 0 8 @m1 1
			endif
	else
		add_sys_msg_start
		add_sys_msg �޷�ʹ����Ʒ���������������½�ȡ!
		set_sys_msg_mode 1 0 0
		msg_info
	
	endif
endproc			
			
				
				