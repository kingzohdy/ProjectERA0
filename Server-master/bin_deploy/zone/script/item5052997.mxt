;朝阳战备箱

proc 1
	strvar num ret zhi
	GET_PACK_SPACE 0 num
	
	if @num < 1
		ADD_SYS_MSG_START
		ADD_SYS_MSG 背包需要1个格子才装得下哦！请先整理背包再开启
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
			ADD_SYS_MSG 成功开启第$@zhi$次【朝阳战备箱】，恭喜你获得1个【朝阳战备】
			SET_SYS_MSG_MODE 3 0 0
			MSG_INFO
		else
			ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5103022 1 1 0
			ADD_SYS_MSG_START
			ADD_SYS_MSG 已成功开启10次以上【朝阳战备箱】，恭喜你获得1个【潮露水晶】
			SET_SYS_MSG_MODE 3 0 0
			MSG_INFO
		endif
	endif
endproc
