;���˽ڴ����
proc 1
	strvar Num
	GET_PACK_SPACE 0 Num

	if Num < 6
		add_sys_msg_start
		add_sys_msg �����ռ䲻��,��Ԥ��6���ո�
		set_sys_msg_mode 4 0 0
		msg_info
		return
		
	else
		
		DEC_ITEM_NUM_INPAK 5052303 1 100
		if 100 = -1
			return
		endif
		ADD_BIND_ITEM_NUM_INPAK 5052301 20 1 20
		ADD_ITEM_NUM_INPAK 5600305 1 1 20
	;	ADD_ITEM_NUM_INPAK 3550017 1 1 20
	;	ADD_ITEM_NUM_INPAK 3550017 1 1 20
		
	;	ADD_ITEM_NUM_INPAK 3850017 1 1 20
	;	ADD_ITEM_NUM_INPAK 3850017 1 1 20
		
	;	ADD_ITEM_NUM_INPAK 3050017 1 1 20
	;	ADD_ITEM_NUM_INPAK 3050017 1 1 20
		
	;	ADD_ITEM_NUM_INPAK 3450017 1 1 20
	;	ADD_ITEM_NUM_INPAK 3450017 1 1 20
		
	;	ADD_ITEM_NUM_INPAK 3750017 1 1 20
	;	ADD_ITEM_NUM_INPAK 3750017 1 1 20
		
		ADD_ITEM_NUM_INPAK 5052928 1 1 20
		ADD_ITEM_NUM_INPAK 5052928 1 1 20
		ADD_ITEM_NUM_INPAK 5043246 1 1 20
		
		ADD_ITEM_NUM_INPAK 5012113 20 1 20
		ADD_ITEM_NUM_INPAK 5043228 1 1 20
		
		
		add_sys_msg_start
		add_sys_msg $_player$�����˽ڴ�������������������Ҹ��ı���������$_player$�����ϣ�
		set_sys_msg_mode 6 0 0
		svr_msg_info
		
	endif
endproc
