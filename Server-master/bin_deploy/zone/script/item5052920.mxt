
;;;;ľ�Ʊ���

proc 1

	strvar ret
	GET_PACK_SPACE 0 ret
	if ret > 0
		DEC_ITEM_NUM_INPAK 5052920 1 10
		if 10 = 0
			rand 20 100
			if 20 < 30
				ADD_BIND_ITEM_NUM_INPAK 5041000 1 1 0
			endif
			if 20 >= 30 and 20 < 35
				ADD_BIND_ITEM_NUM_INPAK 5050000 1 1 0
			endif
			if 20 >= 35 and 20 < 65
				ADD_BIND_ITEM_NUM_INPAK 5055100 1 1 0
			endif
			if 20 >= 65 and 20 < 100
				ADD_BIND_ITEM_NUM_INPAK 5052816 1 1 0
			endif
		endif
	else
		add_sys_msg_start
		add_sys_msg �����ռ䲻�㣬��Ԥ��1���ո��ٿ������ߣ�
		set_sys_msg_mode 4 0 0
		msg_info
	endif

endproc  







