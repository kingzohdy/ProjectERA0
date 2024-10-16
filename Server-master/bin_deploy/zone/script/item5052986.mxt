;感恩之心

proc 1
    strvar num ret
	GET_PACK_SPACE 0 num
	
	if @num < 1
		ADD_SYS_MSG_START
		ADD_SYS_MSG 请空出1个背包格子再开启吧！
		SET_SYS_MSG_MODE 1 0 0
		MSG_INFO
		return
	endif
	
	DEC_ITEM_NUM_INPAK 5052986 1 ret
	
	if  @ret = 0
		rand 10 100
			if 10 < 20
				ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5213411 5 1 0
			endif
			if 10 >= 20 and 10 < 26
				ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5251001 5 1 0
			endif
			if 10 >= 26 and 10 < 46
				ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5060014 2 1 0
			endif
			if 10 >= 46 and 10 < 71
				ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5060023 1 1 0
			endif
			if 10 >= 71 and 10 < 96
				ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5052340 1 1 0
			endif
			if 10 >= 96
				ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5213201 1 1 0
				
				ADD_SYS_MSG_START
				ADD_SYS_MSG 恭喜玩家“$_player$”开启【感恩之心】获得“二级多彩宝石”！
				SET_SYS_MSG_MODE 18 0 0
				svr_msg_info
			endif
	endif
endproc
