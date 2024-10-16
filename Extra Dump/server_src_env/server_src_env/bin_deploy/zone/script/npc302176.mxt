;npc 多林

proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b在过去的一年中，我走遍了世界各地，收集到无数的珍宝，把它们分别装在箱子里，在此国庆佳节之际，向诸位勇士开放。

		option 200 #多林的商店
		
		;task_stat 4635 20 30
		;if @20 = 0 and @30 = 2
			;option 201 #兑换记忆扫描器
		;endif
		
	talk
endproc 

proc 200
	OPEN_SHOP_BY_ID 77
endproc

proc 201
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b初级记忆扫描器直接用雷霆小组调令兑换即可。
	add_talk #b#b高级记忆扫描器，兑换除了需要雷霆小组调令之外还需要#c1865065000银币#n。
	add_talk #b#b初级记忆扫描器可分辨黑暗文明议会初级间谍（若当前区域队伍成员数量≥4人，就会召唤出高级间谍）
	add_talk #b#b高级记忆扫描器可分辨黑暗文明议会高级间谍（若当前区域队伍成员数量≥4人，就会召唤出王牌间谍）
	add_talk #b#b击杀高级间谍可获得#186506英雄纪念币#n
	add_talk #b#b击杀王牌间谍可获得#186506英雄纪念币#n、#cff4e00金券#n
	add_talk #b#b接受#c186506潜伏#n任务，会获得1个雷霆小组调令
	
		option 202 兑换初级记忆扫描器(需要调令)
		option 203 兑换高级记忆扫描器(需要调令+5000银币)
		
	talk
endproc

proc 202
	strvar ret1
	GET_PACK_SPACE 0 ret1
	if @ret1 = 0
		add_sys_msg_start
		add_sys_msg 背包空间不足，无法兑换
		set_sys_msg_mode 4 0 0
		msg_info
		return
	endif
	
	GET_ITEM_NUM_INPAK 5060058 13 15
	if @13 = 0 and @15 >= 1
		DEC_ITEM_NUM_INPAK 5060058 1 14
		if @14 = 0
			ADD_BIND_ITEM_NUM_INPAK 5060059 1 1 16
			add_sys_msg_start
			add_sys_msg 你获得了初级记忆扫描器
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	else
		add_sys_msg_start
		add_sys_msg 背包中没有雷霆小组调令！
		set_sys_msg_mode 3 0 0
		msg_info
	endif
	PLAYER_CLOSE_DIALOG1 0 0
endproc

proc 203
	strvar ret1
	GET_PACK_SPACE 0 ret1
	if @ret1 = 0
		add_sys_msg_start
		add_sys_msg 背包空间不足，无法兑换
		set_sys_msg_mode 4 0 0
		msg_info
		return
	endif
	
	DEC_MONEY_TEST 0 5000 20 18
	if 18 < 0
		add_sys_msg_start
		add_sys_msg 您的银币不足。
		set_sys_msg_mode 4 0 0
		msg_info
		return
	endif	
	
	GET_ITEM_NUM_INPAK 5060058 13 15
	if @13 = 0 and @15 >= 1
		DEC_ITEM_NUM_INPAK 5060058 1 14
		DEC_MONEY 0 5000 20 12
		if @14 = 0 and @12 = 0
			ADD_BIND_ITEM_NUM_INPAK 5060049 1 1 16
			add_sys_msg_start
			add_sys_msg 你获得了高级记忆扫描器
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	else
		add_sys_msg_start
		add_sys_msg 背包中没有雷霆小组调令！
		set_sys_msg_mode 3 0 0
		msg_info
	endif
	PLAYER_CLOSE_DIALOG1 0 0
endproc
