
;;;;�Ŷ����

proc 1

	strvar ret
	GET_PACK_SPACE 0 ret
	if ret >= 5
		DEC_ITEM_NUM_INPAK 5052931 1 10
		if 10 = 0
			ADD_BIND_ITEM_NUM_INPAK 5610217 1 1 0
			ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5052861 1 1 0
			ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5212001 1 1 0
			ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5213201 1 1 0
			ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5219401 1 1 0
		
		endif
	else
		add_sys_msg_start
		add_sys_msg �����ռ䲻�㣬��Ԥ��5���ո��ٿ������ߣ�
		set_sys_msg_mode 4 0 0
		msg_info
	endif

endproc  







