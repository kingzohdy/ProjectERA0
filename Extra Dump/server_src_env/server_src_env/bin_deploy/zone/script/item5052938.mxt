
;;;;����

;��ɫ��ʯ��

proc 1

	strvar ret
	GET_PACK_SPACE 0 ret
	if ret > 0
		DEC_ITEM_NUM_INPAK 5052938 1 10
		if 10 = 0
			rand 20 3
			if 20 = 0
				ADD_BIND_ITEM_NUM_INPAK 5211001 1 1 0
			endif
			if 20 = 1
				ADD_BIND_ITEM_NUM_INPAK 5211101 1 1 0
			endif
			if 20 = 2
				ADD_BIND_ITEM_NUM_INPAK 5213401 1 1 0
			endif
		endif
	else
		add_sys_msg_start
		add_sys_msg �����ռ䲻�㣬��Ԥ��1���ո��ٿ������ߣ�
		set_sys_msg_mode 4 0 0
		msg_info
	endif

endproc  







