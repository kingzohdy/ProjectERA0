;����ս����

proc 1
	strvar num ret zhi
	GET_PACK_SPACE 0 num
	
	if @num < 1
		ADD_SYS_MSG_START
		ADD_SYS_MSG ������Ҫ1�����Ӳ�װ����Ŷ�������������ٿ���
		SET_SYS_MSG_MODE 1 0 0
		MSG_INFO
		return
	endif
	
	DEC_ITEM_NUM_INPAK 5052997 1 ret
	
	if  @ret = 0
		GET_ROLE_VAR 15 8 0 zhi
		if zhi < 10
			add @zhi 1
			SET_ROLE_VAR 15 8 @zhi 0
			ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5052174 1 1 0
			
			GET_ROLE_VAR 15 8 0 zhi
			ADD_SYS_MSG_START
			ADD_SYS_MSG �ɹ�������$@zhi$�Ρ�����ս���䡿����ϲ����1��������ս����
			SET_SYS_MSG_MODE 3 0 0
			MSG_INFO
		else
			ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5103022 1 1 0
			ADD_SYS_MSG_START
			ADD_SYS_MSG �ѳɹ�����10�����ϡ�����ս���䡿����ϲ����1������¶ˮ����
			SET_SYS_MSG_MODE 3 0 0
			MSG_INFO
		endif
	endif
endproc
