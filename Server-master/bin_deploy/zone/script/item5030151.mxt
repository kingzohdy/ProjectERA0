;比赛专用装备包

proc 1
	strvar num ret zhi job ret2
	GET_PACK_SPACE 0 num
	
	if @num < 2
		ADD_SYS_MSG_START
		ADD_SYS_MSG 背包需要2个格子才装得下哦！请先整理背包再开启
		SET_SYS_MSG_MODE 1 0 0
		MSG_INFO
		return
	endif
	
	DEC_ITEM_NUM_INPAK 5030151 1 ret
	
	if  @ret = 0
		ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5030150 1 1 0
		ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5030152 1 1 0
	endif
endproc
