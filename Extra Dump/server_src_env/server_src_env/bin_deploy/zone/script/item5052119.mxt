;����ף�����

proc 1
	strvar num ret ret2 lv
	GET_PACK_SPACE 0 num
	
	if @num < 4
		ADD_SYS_MSG_START
		ADD_SYS_MSG ������Ҫ4�����Ӳ�װ����Ŷ�������������ٿ���
		SET_SYS_MSG_MODE 1 0 0
		MSG_INFO
		return
	endif
	
	DEC_ITEM_NUM_INPAK 5052119 1 ret
	
	if  @ret = 0
		ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5052988 3 1 0
		
		GET_PLAYER_BASE_ATT ret2 0 lv 0
		if ret2 = 0 and lv <= 40
			ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5200045 1 1 0
			
			add_sys_msg_start
			add_sys_msg ��ϲ��ҡ�$_player$������������ף���������á��߽�һ��ϴ��ʯ����
			set_sys_msg_mode 18 0 0
			svr_msg_info
		endif
		if ret2 = 0 and lv >= 41 and lv <= 59
			ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5200046 1 1 0
			
			add_sys_msg_start
			add_sys_msg ��ϲ��ҡ�$_player$������������ף���������á��߽׶���ϴ��ʯ����
			set_sys_msg_mode 18 0 0
			svr_msg_info
		endif
		if ret2 = 0 and lv >= 60 and lv <= 79
			ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5200047 1 1 0
			
			add_sys_msg_start
			add_sys_msg ��ϲ��ҡ�$_player$������������ף���������á��߽�����ϴ��ʯ����
			set_sys_msg_mode 18 0 0
			svr_msg_info
		endif
		if ret2 = 0 and lv >= 80
			ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5200048 1 1 0
			
			add_sys_msg_start
			add_sys_msg ��ϲ��ҡ�$_player$������������ף���������á��߽��ļ�ϴ��ʯ����
			set_sys_msg_mode 18 0 0
			svr_msg_info
		endif
		
		ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5052799 2 1 0
		ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5060023 5 1 0
	endif
endproc
