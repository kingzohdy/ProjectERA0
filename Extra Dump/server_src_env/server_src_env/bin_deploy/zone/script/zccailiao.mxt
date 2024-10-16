;材料


proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b每次米露总是把我赶出来，自己却坐在那而哪都不去，时间城这么大，她还真好意思的。
	add_talk #b#b你有东西需要买吗？没有我就要走了，还要赶路呢。

	option 120 #(买卖)打开商店
	
	strvar RetTsk
	task_stat 3127 1 RetTsk
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
	add_talk #b#b 哇！我的圣诞礼物！让我来拆开看看里面是什么！
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
			task_fini 3127 1
		endif
	endif
endproc 


proc 250
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b给我就行了，我会把它转交给米露的。
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
