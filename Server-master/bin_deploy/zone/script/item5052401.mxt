
;;;�ȷ����

proc 1

	strvar ret
	GET_PACK_SPACE 0 ret
	if ret >= 5
		DEC_ITEM_NUM_INPAK 5052401 1 10
		if 10 = 0
			GET_ID_PLAYER_INFO 0 6 30
			if 30 = 1
			;��
				ADD_BIND_ITEM_NUM_INPAK 3051002 1 1 0
				ADD_BIND_ITEM_NUM_INPAK 3451002 1 1 0
				ADD_BIND_ITEM_NUM_INPAK 3751002 1 1 0
				ADD_BIND_ITEM_NUM_INPAK 3851002 1 1 0
			endif
			if 30 = 2
				ADD_BIND_ITEM_NUM_INPAK 3050002 1 1 0
				ADD_BIND_ITEM_NUM_INPAK 3450002 1 1 0
				ADD_BIND_ITEM_NUM_INPAK 3750002 1 1 0
				ADD_BIND_ITEM_NUM_INPAK 3850002 1 1 0

			endif
			
			ADD_BIND_ITEM_NUM_INPAK 3550002 1 1 0
		;	ADD_BIND_ITEM_NUM_INPAK 3651001 1 1 0
		endif
	else
		add_sys_msg_start
		add_sys_msg �����ռ䲻�㣬��Ԥ��5���ո��ٿ������ߣ�
		set_sys_msg_mode 4 0 0
		msg_info
	endif

endproc  







