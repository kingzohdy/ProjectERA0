;�߽�ϴ��ʯ��

proc 1
    strvar num ret lv
	GET_PACK_SPACE 0 num
	
	if @num < 1
		ADD_SYS_MSG_START
		ADD_SYS_MSG ������Ҫ1�����Ӳ�װ����Ŷ�������������ٿ���
		SET_SYS_MSG_MODE 3 0 0
		MSG_INFO
		return
	endif
	
	DEC_ITEM_NUM_INPAK 5052267 1 ret
	
	if  @ret = 0
		GET_PLAYER_BASE_ATT 0 0 lv 0
		if lv < 40
			ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5200045 1 1 0
			
			add_sys_msg_start
			add_sys_msg ��ϲ��ҡ�$_player$���������߼�ϴ��ʯ������á��߽�һ��ϴ��ʯ����
			set_sys_msg_mode 18 0 0
			svr_msg_info
		endif
		if lv >= 40 and lv < 60
			ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5200046 1 1 0
			
			add_sys_msg_start
			add_sys_msg ��ϲ��ҡ�$_player$���������߼�ϴ��ʯ������á��߽׶���ϴ��ʯ����
			set_sys_msg_mode 18 0 0
			svr_msg_info
		endif
		if lv >= 60 and lv < 80
			ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5200047 1 1 0
			
			add_sys_msg_start
			add_sys_msg ��ϲ��ҡ�$_player$���������߼�ϴ��ʯ������á��߽�����ϴ��ʯ����
			set_sys_msg_mode 18 0 0
			svr_msg_info
		endif
		if lv >= 80 and lv < 120
			ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5200048 1 1 0
			
			add_sys_msg_start
			add_sys_msg ��ϲ��ҡ�$_player$���������߼�ϴ��ʯ������á��߽��ļ�ϴ��ʯ����
			set_sys_msg_mode 18 0 0
			svr_msg_info
		endif
		if lv >= 120
			ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5200049 1 1 0
			
			add_sys_msg_start
			add_sys_msg ��ϲ��ҡ�$_player$���������߼�ϴ��ʯ������á��߽��弶ϴ��ʯ����
			set_sys_msg_mode 18 0 0
			svr_msg_info
		endif
	endif
endproc
