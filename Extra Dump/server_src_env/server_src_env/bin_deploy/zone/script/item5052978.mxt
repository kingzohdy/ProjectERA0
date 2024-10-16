;预创抽奖礼包

proc 1
    strvar num ret zhi xianzhi
	GET_PACK_SPACE 0 num
	
	if @num < 3
		ADD_SYS_MSG_START
		ADD_SYS_MSG 背包需要3个格子才装得下哦！请先整理背包再开启
		SET_SYS_MSG_MODE 3 0 0
		MSG_INFO
		return
	endif
	
	DEC_ITEM_NUM_INPAK 5052978 1 ret
	
	if  @ret = 0
		ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5251001 10 1 0
		ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5052817 1 1 0
		rand zhi 10000
		if zhi < 5
			ADD_BIND_ITEM_NUM_INPAK 5060091 1 1 xianzhi
			if xianzhi = -2
				ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5250102 1 1 0
			endif
		endif
		if zhi >= 5 and zhi < 505
			ADD_BIND_ITEM_NUM_INPAK 5060092 1 1 xianzhi
			if xianzhi = -2
				ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5250102 1 1 0
			endif
		endif
		if zhi >= 505 and zhi < 605
			ADD_BIND_ITEM_NUM_INPAK 5060093 1 1 xianzhi
			if xianzhi = -2
				ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5250102 1 1 0
			endif
		endif
		if zhi >= 605 and zhi < 4000
			ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5030129 1 1 0
		endif
		if zhi >= 4000 and zhi < 6000
			ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5250102 1 1 0
		endif
		if zhi >= 6000 and zhi < 9000
			ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 3551001 1 1 0
		endif
		if zhi >= 9000
			ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5610417 1 1 0
		endif
	endif
endproc
