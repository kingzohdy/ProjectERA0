;�������� 5233720
proc 1

	strvar ret
	GET_PACK_SPACE 0 ret
	if ret >= 2
		DEC_ITEM_NUM_INPAK 5233720 1 10
		if 10 = 0
			rand 40 100
				if 40 < 60
				ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5233810 1 1 0

				add_sys_msg_start
				add_sys_msg ���1��Ǳ����¶
				set_sys_msg_mode 3 0 0
				MSG_INFO
				endif
				if 40 >= 60 and 40 < 90
				ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5233810 2 1 0

				add_sys_msg_start
				add_sys_msg ���2��Ǳ����¶
				set_sys_msg_mode 3 0 0
				MSG_INFO
				endif
				if 40 >= 90 and 40 < 95
				ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5233810 3 1 0

				add_sys_msg_start
				add_sys_msg ���3��Ǳ����¶
				set_sys_msg_mode 3 0 0
				MSG_INFO
				endif
				if 40 >= 95
				ADD_ITEM_NUM_INPAK_NOLIMIT 5233801 1 1 0
				
				add_sys_msg_start
				add_sys_msg ���1����ˮ
				set_sys_msg_mode 3 0 0
				MSG_INFO
				endif				
		endif
	else
		add_sys_msg_start
		add_sys_msg �����ռ䲻�㣬��Ԥ��2���ո��ٿ������ߣ�
		set_sys_msg_mode 4 0 0
		msg_info
	endif

endproc  
