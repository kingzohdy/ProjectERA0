;3����ʯ��Ƕ����Ƭ(��)


proc 1

	GET_ITEM_NUM_INPAK 5229102 1 11
	if 11 < 5
		add_sys_msg_start
		add_sys_msg ���ϲ��㣬�����5����
		set_sys_msg_mode 4 0 0
		map_msg_info
	ENDIF
	if 11 >= 5
		ADD_ITEM_NUM_INPAK 5219102 1 1 12
		IF 12 = 0
			DEC_ITEM_NUM_INPAK 5229102 5 1
		Else
		add_sys_msg_start
		add_sys_msg ������������Ԥ��1����λ��
		set_sys_msg_mode 4 0 0
		map_msg_info
		ENDIF
	ENDIF
endproc		