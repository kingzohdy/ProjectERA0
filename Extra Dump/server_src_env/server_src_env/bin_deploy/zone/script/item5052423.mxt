;5052423	����ʱװ��30��

proc 1
	strvar ret
	GET_PACK_SPACE 0 ret
	if ret >= 5
		DEC_ITEM_NUM_INPAK 5052423 1 10
		if 10 = 0
			ADD_BIND_ITEM_NUM_INPAK 3050035 1 1 0
			ADD_BIND_ITEM_NUM_INPAK 3450035 1 1 0
			ADD_BIND_ITEM_NUM_INPAK 3750035 1 1 0
			ADD_BIND_ITEM_NUM_INPAK 3850035 1 1 0
			ADD_BIND_ITEM_NUM_INPAK 3550035 1 1 0
		endif
	else
		add_sys_msg_start
		add_sys_msg �����ռ䲻�㣬��Ԥ��5���ո��ٿ������ߣ�
		set_sys_msg_mode 3 0 0
	endif


endproc 