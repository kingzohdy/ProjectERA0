;����ս����

proc 1
	strvar num ret zhi job ret2
	GET_PACK_SPACE 0 num
	
	if @num < 1
		ADD_SYS_MSG_START
		ADD_SYS_MSG ������Ҫ1�����Ӳ�װ����Ŷ�������������ٿ���
		SET_SYS_MSG_MODE 1 0 0
		MSG_INFO
		return
	endif
	
	DEC_ITEM_NUM_INPAK 5052140 1 ret
	
	if  @ret = 0
		GET_PLAYER_BASE_ATT ret2 0 0 job
		if ret2 = 0 and job = 17
			ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 2721907 1 1 0
			call 100
		endif
		if ret2 = 0 and job = 18
			ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 2731907 1 1 0
			call 100
		endif
		if ret2 = 0 and job = 19
			ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 2741907 1 1 0
			call 100
		endif
		if ret2 = 0 and job = 20
			ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 2751907 1 1 0
			call 100
		endif
		if ret2 = 0 and job = 33
			ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 2761907 1 1 0
			call 100
		endif
		if ret2 = 0 and job = 34
			ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 2771907 1 1 0
			call 100
		endif
		if ret2 = 0 and job = 35
			ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 2781907 1 1 0
			call 100
		endif
		if ret2 = 0 and job = 36
			ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 2791907 1 1 0
			call 100
		endif
	endif
endproc

proc 100
	ADD_SYS_MSG_START
	ADD_SYS_MSG ��ϲ��ҡ�$_player$���������������������Ʒ��á�ʷʫ��ʤ��������������
	SET_SYS_MSG_MODE 18 0 0
	SVR_MSG_INFO
endproc
