;;;;;��ѩ�µ�Ů��  ����ɯ



proc 2
	GET_PLAYER_ID 30 0
	PLAYER_RIDE_ON @30 35 36
	if 35 = 0
		add_sys_msg_start
		add_sys_msg �������ŵ�����Ǹ�С��ü�������������ٱ��˼Ұɣ�
		set_sys_msg_mode 4 0 0
		msg_info
		return
	endif


;	NEW_OWNER_TMP_ITEM 1460062 0 0 0 0 0 0 0 0 0 1
	APPLY_TMP_ITEM 1460077 0 1
	
endproc
proc 6
	GET_PLAYER_ID 30 0
	PLAYER_RIDE_ON @30 35 36
	if 35 = 0
	
		PLAYER_COLLECT_BREAK
		add_sys_msg_start
		add_sys_msg �������ŵ�����Ǹ�С��ü�������������ٱ��˼Ұɣ�
		set_sys_msg_mode 4 0 0
		msg_info
		return
	endif
	

endproc 








