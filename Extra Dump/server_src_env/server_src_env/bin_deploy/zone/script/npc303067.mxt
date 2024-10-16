proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b你好，我来自议会。
	add_talk #b#b时间城议会为了鼓励先行者们去探险、发掘新能源，联合基尔特商会建立了一系列的奖励措施。
	add_talk #b#b其中一种是正义点数，击败 #c186506野外BOSS和副本BOSS#n 即可获得，并作为一种货币储存在人物界面奖章一页中。
	add_talk #b#b搜集正义点数然后在这里购买心仪的物品吧。
	add_talk #b#b勇者，愿你变得越来越强大！
	
	strvar LV
	GET_PLAYER_BASE_ATT 1 1 LV 1
	
	if @LV < 70
		option 101 #(买卖)【正义点数】商店
	endif
	if @LV >= 70 and @LV  <= 85
		option 102 #(买卖)【正义点数】商店
	endif
	
		option 200 1个【星辉宝钻】兑换1个【进阶凝露】
		option 210 10个【星辉宝钻】兑换10个【进阶凝露】
		option 220 50个【星辉宝钻】兑换50个【进阶凝露】
	
	talk
endproc

proc 101
	OPEN_SHOP_BY_ID 10000
	
	task_fini 2266 1
endproc
proc 102
	OPEN_SHOP_BY_ID 10001
	
	task_fini 2266 1
endproc

proc 200
	strvar ret unl
	PKG_IS_UNLOCK ret unl
	if ret = 0 and unl = 1
		choose 2 201 202 　　　兑换1个【进阶凝露】需要1个【星辉宝钻】　　是否确认兑换？
	endif
endproc

proc 201
	strvar ret1 num ret2 pmid num1
	GET_ITEM_NUM_INPAK 5200020 ret1 num
	GET_PACK_SPACE 0 num1
	

	if @num1 < 1
		ADD_SYS_MSG_START
		ADD_SYS_MSG 背包需要1个格子才装得下哦！请先整理背包再兑换
		SET_SYS_MSG_MODE 3 0 0
		MSG_INFO
		return
	endif
		
		
	if ret1 = 0 and num > 0
		DEC_ITEM_NUM_INPAK 5200020 1 ret2
		if ret2 = 0
			ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5233512 1 1 0
				
			add_sys_msg_start
			add_sys_msg 你已成功兑换了1个【进阶凝露】
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	else
		add_sys_msg_start
		add_sys_msg 你身上的星辉宝钻不足，暂时无法兑换
		set_sys_msg_mode 3 0 0
		msg_info
	endif
endproc

proc 210
	strvar ret unl
	PKG_IS_UNLOCK ret unl
	if ret = 0 and unl = 1
		choose 2 211 202 　　　兑换10个【进阶凝露】需要10个【星辉宝钻】　　是否确认兑换？
	endif
endproc

proc 211
	strvar ret1 num ret2 pmid num1
	GET_ITEM_NUM_INPAK 5200020 ret1 num
	GET_PACK_SPACE 0 num1
	
	if @num1 < 1
		ADD_SYS_MSG_START
		ADD_SYS_MSG 背包需要1个格子才装得下哦！请先整理背包再兑换
		SET_SYS_MSG_MODE 3 0 0
		MSG_INFO
		return
	endif
		
		
	if ret1 = 0 and num > 9
		DEC_ITEM_NUM_INPAK 5200020 10 ret2
		if ret2 = 0
			ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5233512 10 1 0
			
			add_sys_msg_start
			add_sys_msg 你已成功兑换了10个【进阶凝露】
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	else
		add_sys_msg_start
		add_sys_msg 你身上的星辉宝钻不足，暂时无法兑换
		set_sys_msg_mode 3 0 0
		msg_info
	endif
endproc

proc 220
	strvar ret unl
	PKG_IS_UNLOCK ret unl
	if ret = 0 and unl = 1
		choose 2 221 202 　　　兑换50个【进阶凝露】需要50个【星辉宝钻】　　是否确认兑换？
	endif
endproc

proc 221
	strvar ret1 num ret2 pmid num1
	GET_ITEM_NUM_INPAK 5200020 ret1 num
	GET_PACK_SPACE 0 num1
	
	if @num1 < 1
		ADD_SYS_MSG_START
		ADD_SYS_MSG 背包需要1个格子才装得下哦！请先整理背包再兑换
		SET_SYS_MSG_MODE 3 0 0
		MSG_INFO
		return
	endif
		
		
	if ret1 = 0 and num > 49
		DEC_ITEM_NUM_INPAK 5200020 50 ret2
		if ret2 = 0
			ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5233512 50 1 0
			
			add_sys_msg_start
			add_sys_msg 你已成功兑换了50个【进阶凝露】
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	else
		add_sys_msg_start
		add_sys_msg 你身上的星辉宝钻不足，暂时无法兑换
		set_sys_msg_mode 3 0 0
		msg_info
	endif
endproc
