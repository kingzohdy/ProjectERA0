;铁匠


proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b瞧瞧这坚实的肌肉，这就是铁匠的灵魂！看看这些被我汗水浸透的装备，因此充满了爱的温度。
	add_talk #b#b为了不让生命留下遗憾，尽情的在此挥霍吧！

	option 120 #(买卖)打开商店
		strvar RetTsk
	task_stat 3129 1 RetTsk
	if RetTsk = 2
		option 200 这是圣诞老公公给你的礼物
	endif
	
	strvar Ret
	task_stat 3674 1 Ret
	if Ret = 2
		option 250 我替公会送来了你要的生产物资
	endif

	task_stat 4147 1 50
	if 50 = 2
		option 300 #我替大马丁送来了你订购的物资
	endif
	
	talk
endproc 	
proc 120
	OPEN_NPC_SHOP 
endproc 
proc 200 
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b 哈哈哈！这我也有圣诞礼物啊！哈哈哈哈！
		option 210 #交出圣诞礼物
	talk
endproc 
proc 210
	GET_ITEM_NUM_INPAK 5300844 0 11
	if 11 < 1
		add_sys_msg_start
		add_sys_msg 你身上没有这件物品！
		set_sys_msg_mode 1 0 0
		msg_info
	ENDIF
	if 11 >= 1
			
		dec_item_num_inpak 5300844 1 30
		if 30 != -1
			task_fini 3129 1
		endif
	endif
endproc 

proc 250
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b公会的送货时间还是一如既往的准时啊。
		option 251 #交出生产物资
	talk
endproc 
proc 251
	GET_ITEM_NUM_INPAK 5300871 0 11
	if 11 < 1
		add_sys_msg_start
		add_sys_msg 你身上没有这件物品！
		set_sys_msg_mode 1 0 0
		msg_info
	ENDIF
	if 11 >= 1
			
		dec_item_num_inpak 5300871 1 30
		if 30 != -1
			task_fini 3674 1
		endif
	endif
endproc


proc 300
	GET_ITEM_NUM_INPAK 5301003 0 11
	if 11 < 1
	
		add_sys_msg_start
		add_sys_msg 你身上没有这件物品！
		set_sys_msg_mode 1 0 0
		msg_info
	else
	
		dec_item_num_inpak 5301003 1 30
		if 30 != -1
			task_fini 4147 1
		endif
	endif
endproc






