
;С��
proc 1
	strvar Ret
	task_stat 2104 1 Ret
	if Ret = 2
		strvar Num
		GET_ITEM_NUM_INPAK 5300892 1 Num
		if Num = 0
			add_sys_msg_start
			add_sys_msg ��û���㹻�����������⣡
			set_sys_msg_mode 3 0 0
			msg_info
		else
			strvar Reti
			DEC_ITEM_NUM_INPAK 5300892 1 Reti
			if Reti = 0
				ADD_ITEM_NUM_INPAK 5300893 3 1 0
				; ADD_ITEM_NUM_INPAK 5300891 1 1 0
				
				strvar rac
				GET_ID_PLAYER_INFO 0 7 rac
				if rac = 17 or rac = 33
				;սʿ
					add_sys_msg_start
					add_sys_msg ���������Ҷ��Ҷ��Ҷ��硭��
					set_sys_msg_mode 3 0 0
					msg_info
				endif
				if rac = 18 or rac = 34
				;��ʦ
					add_sys_msg_start
					add_sys_msg �ұ������ұ������������򡭴򡭴򡭡�
					set_sys_msg_mode 3 0 0
					msg_info
				endif
				if rac = 19 or rac = 35
				;ǹ��
					add_sys_msg_start
					add_sys_msg ��誡������ұ�̢��ɣ�
					set_sys_msg_mode 3 0 0
					msg_info
				endif
				if rac = 20 or rac = 36
				;����
					add_sys_msg_start
					add_sys_msg �����ֽ���������գ�һ��������ת���ȰȰȡ���
					set_sys_msg_mode 3 0 0
					msg_info
				endif
			endif
		endif
	endif
		
endproc  


