;��ͼϻ


proc 1

	ADD_ITEM_BAT_INPAK 3 5062000 1 5062001 1 5062002 1 10 11 12 13 14 15 16 17 18 19 1 101
	IF 101 = 0 
		DEC_ITEM_NUM_INPAK 5062051 1 1
	Else
		add_sys_msg_start
		add_sys_msg ����λ�ò��㣬��Ԥ��3����λ��
		set_sys_msg_mode 4 0 0
		msg_info
	ENDIF
endproc		