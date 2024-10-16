;多彩宝石礼包

proc 1
    strvar num ret
	GET_PACK_SPACE 0 num
	
	if @num < 3
		ADD_SYS_MSG_START
		ADD_SYS_MSG 背包空间不足，请预留3个空格再开启道具！
		SET_SYS_MSG_MODE 1 0 0
		MSG_INFO
		return
	endif
	
	DEC_ITEM_NUM_INPAK 5052266 1 ret
	
	if  @ret = 0
		ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5012142 1 1 0
		ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5012143 1 1 0
		ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5213201 1 1 0
	endif
endproc
