;牧牛预赛,佳男
;子里
proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b牛仔很忙决赛结束后，就可以来我这查询比赛成绩。由于参赛人数众多，我只公布用时最少速度最快的前10名选手的成绩。
	add_talk #b#b点击下方的“查询决赛成绩，领取奖励”选项就可看到前10名的成绩榜单，凡是榜上有名的玩家，都可以在我这里领取一份排名奖励。
	add_talk #b#b温馨提示：请在决赛结束后24小时内领取排名奖励，排名奖励非常丰厚，千万不要错过哟。
	
	option 110 查询决赛成绩，领取奖励
	
		strvar Ret num1
	GET_ITEM_NUM_INPAK 5300926 1 num1
	if num1 > 0
		TASK_STAT 3305 1 Ret
		if Ret = 2
			option 700 曲别针换别墅
		endif
	endif
	
	
	talk
endproc 
proc 110
	OPEN_COW_FINAL_RANGE_UI 1
endproc
proc 700
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b如果当真按照书里的内容催动咒语，只怕伤人伤已！不知道是谁出版了这篇误人子弟的东西……
	add_talk #b#b跟我交换吧，你也不希望有人因你的行为而受到伤害，对吧？

		option 701 #交换

	talk
	
endproc
proc 701
	
	strvar ItmNum Num Rad
	GET_ITEM_NUM_INPAK 5300926 1 ItmNum
	if ItmNum > 0
			
		GET_PACK_SPACE 0 Num
		if Num > 0
			DEC_ITEM_NUM_INPAK 5300926 1 1
			
			rand Rad 100
			if @Rad < 80
				ADD_ITEM_NUM_INPAK 5300927 1 0 0
			endif
			if @Rad >= 80 and @Rad < 95
				ADD_ITEM_NUM_INPAK 5300928 1 0 0
			endif
			if @Rad >= 95 and @Rad < 99
				ADD_ITEM_NUM_INPAK 5300929 1 0 0
			endif
			if @Rad = 99
				ADD_ITEM_NUM_INPAK 5300930 1 0 0
			endif
		else
			add_sys_msg_start
			add_sys_msg 背包空间不足！
			set_sys_msg_mode 1 0 0
			msg_info
		endif
	else
		add_sys_msg_start
		add_sys_msg 你身上没有可以交换的物品！
		set_sys_msg_mode 1 0 0
		msg_info
	endif
endproc 
