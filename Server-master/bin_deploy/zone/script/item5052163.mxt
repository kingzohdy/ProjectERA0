;亚军战备箱

proc 1
	strvar num ret zhi job ret2
	GET_PACK_SPACE 0 num
	
	if @num < 1
		ADD_SYS_MSG_START
		ADD_SYS_MSG 背包需要1个格子才装得下哦！请先整理背包再开启
		SET_SYS_MSG_MODE 1 0 0
		MSG_INFO
		return
	endif
	
	DEC_ITEM_NUM_INPAK 5052163 1 ret
	
	if  @ret = 0
		GET_PLAYER_BASE_ATT ret2 0 0 job
		if ret2 = 0 and job = 17
			ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 2725009 1 1 0
			call 100
		endif
		if ret2 = 0 and job = 18
			ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 2735009 1 1 0
			call 100
		endif
		if ret2 = 0 and job = 19
			ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 2745009 1 1 0
			call 100
		endif
		if ret2 = 0 and job = 20
			ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 2755009 1 1 0
			call 100
		endif
		if ret2 = 0 and job = 33
			ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 2765009 1 1 0
			call 100
		endif
		if ret2 = 0 and job = 34
			ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 2775009 1 1 0
			call 100
		endif
		if ret2 = 0 and job = 35
			ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 2785009 1 1 0
			call 100
		endif
		if ret2 = 0 and job = 36
			ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 2795009 1 1 0
			call 100
		endif
	endif
endproc

proc 100
	ADD_SYS_MSG_START
	ADD_SYS_MSG 恭喜玩家“$_player$”开启跨服公会争霸赛奖品获得“英雄的荣耀凯旋护符”！
	SET_SYS_MSG_MODE 18 0 0
	SVR_MSG_INFO
endproc
