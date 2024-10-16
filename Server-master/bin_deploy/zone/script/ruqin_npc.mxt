

proc 1

	dialog 1
	add_talk $_npc$:
	add_talk #b#b宝箱中满载着被“肆掠者”夺走的珍贵财产！
	add_talk #b#b
	add_talk #b#b#c186506为了感谢大家的努力，每人可从“肆掠者宝箱”中领取一次奖励#n
	
		strvar NumRet
		GET_PLAYER_DAILY_FLAG 1 8 NumRet
		if NumRet = 0
			option 150 #领取奖励
		endif
	talk
endproc 

proc 150 
	strvar lv num
	GET_PLAYER_BASE_ATT 1 1 lv 1
	GET_PACK_SPACE 0 num
	if lv <= 40
		if @num < 2
			ADD_SYS_MSG_START
			ADD_SYS_MSG 背包需要2个格子才装得下哦！请先整理背包再来领取奖励
			SET_SYS_MSG_MODE 3 0 0
			MSG_INFO
		else
			ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5200030 10 1 0
			ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5200040 10 1 0
			
			SET_PLAYER_DAILY_FLAG 0 8 1
		endif
	endif
	; if lv > 10 and lv <= 20
	; endif
	; if lv > 20 and lv <= 30
	; endif
	; if lv > 30 and lv <= 40
	; endif
	if lv > 40 and lv <= 60
		if @num < 2
			ADD_SYS_MSG_START
			ADD_SYS_MSG 背包需要2个格子才装得下哦！请先整理背包再来领取奖励
			SET_SYS_MSG_MODE 3 0 0
			MSG_INFO
		else
			ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5200031 8 1 0
			ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5200041 8 1 0
			
			SET_PLAYER_DAILY_FLAG 0 8 1
		endif
	endif

	if lv > 60 and lv <= 79
		if @num < 3
			ADD_SYS_MSG_START
			ADD_SYS_MSG 背包需要3个格子才装得下哦！请先整理背包再来领取奖励
			SET_SYS_MSG_MODE 3 0 0
			MSG_INFO
		else
			ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5200032 4 1 0
			ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5200042 4 1 0
			ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5200020 1 1 0
			
			SET_PLAYER_DAILY_FLAG 0 8 1
		endif
	endif

	if lv > 79 and lv <= 100
		if @num < 3
			ADD_SYS_MSG_START
			ADD_SYS_MSG 背包需要3个格子才装得下哦！请先整理背包再来领取奖励
			SET_SYS_MSG_MODE 3 0 0
			MSG_INFO
		else
			ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5200033 4 1 0
			ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5200043 4 1 0
			ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5200020 1 1 0
				
			SET_PLAYER_DAILY_FLAG 0 8 1
		endif
	endif
	
endproc 


