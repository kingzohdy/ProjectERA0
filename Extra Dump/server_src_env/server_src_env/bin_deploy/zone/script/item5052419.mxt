;5052419	�Ǽ�ʱװ��7��

proc 1
	strvar ret
	GET_PACK_SPACE 0 ret
	if ret >= 5
		DEC_ITEM_NUM_INPAK 5052419 1 10
		if 10 = 0
			ADD_BIND_ITEM_NUM_INPAK 3050025 1 1 0
			ADD_BIND_ITEM_NUM_INPAK 3450025 1 1 0
			ADD_BIND_ITEM_NUM_INPAK 3750025 1 1 0
			ADD_BIND_ITEM_NUM_INPAK 3850025 1 1 0
			ADD_BIND_ITEM_NUM_INPAK 3550025 1 1 0
		endif
	else
		add_sys_msg_start
		add_sys_msg �����ռ䲻�㣬��Ԥ��5���ո��ٿ������ߣ�
		set_sys_msg_mode 3 0 0
	endif


endproc 