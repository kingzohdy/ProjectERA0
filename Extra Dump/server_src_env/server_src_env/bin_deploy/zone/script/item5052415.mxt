;5052415	����ʱװ������

proc 1
	strvar ret
	GET_PACK_SPACE 0 ret
	if ret >= 4
		DEC_ITEM_NUM_INPAK 5052415 1 10
		if 10 = 0
			ADD_BIND_ITEM_NUM_INPAK 3050036 1 1 0
			ADD_BIND_ITEM_NUM_INPAK 3450036 1 1 0
			ADD_BIND_ITEM_NUM_INPAK 3750036 1 1 0
			ADD_BIND_ITEM_NUM_INPAK 3850036 1 1 0
		endif
	else
		add_sys_msg_start
		add_sys_msg �����ռ䲻�㣬��Ԥ��4���ո��ٿ������ߣ�
		set_sys_msg_mode 3 0 0
	endif


endproc 