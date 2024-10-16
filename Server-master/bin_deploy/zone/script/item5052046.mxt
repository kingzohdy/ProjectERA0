;宠物强化超值组

proc 1
    strvar num ret
	GET_PACK_SPACE 0 num
	
	if @num < 3
		ADD_SYS_MSG_START
		ADD_SYS_MSG 背包需要3个格子才装得下哦！请先整理背包再开启
		SET_SYS_MSG_MODE 3 0 0
		MSG_INFO
		return
	endif
	
	DEC_ITEM_NUM_INPAK 5052046 1 ret
	
	if  @ret = 0
		ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5233500 50 1 0
		ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5233000 30 1 0
		ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5219601 20 1 0
	endif
endproc