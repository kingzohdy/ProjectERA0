;ս�������շ���Ƭ


proc 1

	GET_ITEM_NUM_INPAK 5209012 1 11
	if 11 < 5
		add_sys_msg_start
		add_sys_msg ���ϲ��㣬�����5����
		set_sys_msg_mode 4 0 0
		map_msg_info
	ENDIF
	if 11 >= 5
		ADD_ITEM_NUM_INPAK 5209010 1 1 12
		IF 12 = 0
			DEC_ITEM_NUM_INPAK 5209012 5 1
		Else
		add_sys_msg_start
		add_sys_msg ������������Ԥ��һ����λ��
		set_sys_msg_mode 4 0 0
		map_msg_info
		ENDIF
	ENDIF
endproc		