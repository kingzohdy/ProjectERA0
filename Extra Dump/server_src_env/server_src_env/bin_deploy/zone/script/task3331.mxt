; ���껪-�ᱦ

Proc 3
	strvar num ret
	rand num 1000
		if num < 100
			ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5052988 1 1 ret
			
			if ret = 0
				add_sys_msg_start
				add_sys_msg ��ϲ��ҡ�$_player$����ɡ����껪-�ᱦ�������á�������ʯ������
				set_sys_msg_mode 18 0 0
				svr_msg_info
			endif
		endif
		if num >= 100 and num < 110
			ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5052989 1 1 ret
			
			if ret = 0
				add_sys_msg_start
				add_sys_msg ��ϲ��ҡ�$_player$����ɡ����껪-�ᱦ�������á��ļ���ʯ������
				set_sys_msg_mode 18 0 0
				svr_msg_info
			endif
		endif
		if num >= 110 and num < 111
			ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5052990 1 1 ret
			
			if ret = 0
				add_sys_msg_start
				add_sys_msg ��ϲ��ҡ�$_player$����ɡ����껪-�ᱦ�������á��弶��ʯ������
				set_sys_msg_mode 18 0 0
				svr_msg_info
			endif
		endif
		if num >= 111
		endif
endproc
