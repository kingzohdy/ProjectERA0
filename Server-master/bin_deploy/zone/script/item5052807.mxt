;�ص䱦��
proc 1
	strvar Num
	GET_PACK_SPACE 0 Num
	if Num < 1
		add_sys_msg_start
		add_sys_msg �����������������Ʒ����Ԥ��1�����ϱ����ո�λ���ٿ�����
		set_sys_msg_mode 1 0 0
		msg_info
		return
	else
		strvar Rad1 
		DEC_ITEM_NUM_INPAK 5052807 1 100
		if 100 = -1
			return
		endif
		
		rand Rad1 100
		if Rad1 < 25
			ADD_ITEM_NUM_INPAK_NOLIMIT 5233000 40 1 0
			add_sys_msg_start
			add_sys_msg ��ϲ��ҡ�$_player$���������ص䱦�䡿���40�����û���ʵ����
			set_sys_msg_mode 18 0 0
			svr_msg_info
		endif
		if Rad1 >= 25 and Rad1 < 50
			ADD_ITEM_NUM_INPAK_NOLIMIT 5060014 120 1 0
			add_sys_msg_start
			add_sys_msg ��ϲ��ҡ�$_player$���������ص䱦�䡿���120������ʯ��Ƭ����
			set_sys_msg_mode 18 0 0
			svr_msg_info
		endif
		if Rad1 >= 50 and Rad1 < 68
			ADD_ITEM_NUM_INPAK_NOLIMIT 5200020 60 1 0
			add_sys_msg_start
			add_sys_msg ��ϲ��ҡ�$_player$���������ص䱦�䡿���60�����ǻԱ��ꡱ��
			set_sys_msg_mode 18 0 0
			svr_msg_info
		endif
		if Rad1 >= 68 and Rad1 < 73
			ADD_ITEM_NUM_INPAK_NOLIMIT 5052954 1 1 0
			add_sys_msg_start
			add_sys_msg ��ϲ��ҡ�$_player$���������ص䱦�䡿��á�100���������
			set_sys_msg_mode 18 0 0
			svr_msg_info
		endif
		if Rad1 >= 73 and Rad1 < 83
			ADD_ITEM_NUM_INPAK_NOLIMIT 5250404 1 1 0
			add_sys_msg_start
			add_sys_msg ��ϲ��ҡ�$_player$���������ص䱦�䡿��á��Ͼ�����������
			set_sys_msg_mode 18 0 0
			svr_msg_info
		endif
		if Rad1 >= 83 and Rad1 < 93
			ADD_ITEM_NUM_INPAK_NOLIMIT 5251015 1 1 0
			add_sys_msg_start
			add_sys_msg ��ϲ��ҡ�$_player$���������ص䱦�䡿���ϡ�б�����������Ƥ������
			set_sys_msg_mode 18 0 0
			svr_msg_info
		endif
		if Rad1 >= 93 and Rad1 < 98
			ADD_ITEM_NUM_INPAK 5600208 1 0 20
			if 20 = 0
				add_sys_msg_start
				add_sys_msg ��ϲ��ҡ�$_player$���������ص䱦�䡿���ϡ�������ʿ֮�ꡱ��
				set_sys_msg_mode 18 0 0
				svr_msg_info
			endif
			if 20 = -2
				ADD_ITEM_NUM_INPAK_NOLIMIT 5233000 40 1 0
				add_sys_msg_start
				add_sys_msg ��ϲ��ҡ�$_player$���������ص䱦�䡿���40�����û���ʵ����
				set_sys_msg_mode 18 0 0
				svr_msg_info
			endif
		endif
		if Rad1 >= 98
			ADD_ITEM_NUM_INPAK 5610305 1 0 20
			if 20 = 0
				add_sys_msg_start
				add_sys_msg ��ϲ��ҡ�$_player$���������ص䱦�䡿���ϡ�г����������
				set_sys_msg_mode 18 0 0
				svr_msg_info
			endif
			if 20 = -2
				ADD_ITEM_NUM_INPAK_NOLIMIT 5233000 40 1 0
				add_sys_msg_start
				add_sys_msg ��ϲ��ҡ�$_player$���������ص䱦�䡿���40�����û���ʵ����
				set_sys_msg_mode 18 0 0
				svr_msg_info
			endif
		endif
	endif
endproc
