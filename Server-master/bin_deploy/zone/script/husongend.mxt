;护送终点

proc 1
	dialog 1
	add_talk $_npc$:
	add_talk #b#b我们基尔特商会垄断了整个世界的商业运输，只要有人迹的地方就有我们的身影。
	
		option 120 #(买卖)打开商店
		strvar ret num map
		task_stat 1096 1 ret
		GET_ITEM_NUM_INPAK 5300945 1 num
		GET_PLAYER_MAPINFO 0 map 0 0 0
		
		if ret = 2 and num = 0 and map = 3400
			
			option 130 #领取商运物资
		
		endif
		strvar num1
		GET_ITEM_NUM_INPAK 5300964 1 num1
		if num1 > 0 and map = 3400
			option 140 #交出“合作申请表”
		endif
	talk
endproc
proc 120
	OPEN_NPC_SHOP 
	
endproc

proc 130
	strvar retin retcar
	in_dart_car retin retcar 0
	if retin = 0 and retcar = 1

		strvar num
		GET_ITEM_NUM_INPAK 5300944 1 num
		if num > 0
			strvar ret
			dec_item_num_inpak 5300944 1 ret
			
			
			if ret = 0
				ADD_ITEM_NUM_INPAK 5300945 1 0 0
			endif
		else
			add_sys_msg_start
			add_sys_msg 你身上没有这件物品！
			set_sys_msg_mode 4 0 0
			msg_info
		
		endif
	else
		add_sys_msg_start
		add_sys_msg 领取商运物资时，人物需要搭载在货车上
		set_sys_msg_mode 4 0 0
		msg_info
	
	endif
	
	
endproc

proc 140
	
	DEC_ITEM_NUM_INPAK 5300964 1 10
	if 10 = 0
		task_fini 1095 1
	endif
endproc 
