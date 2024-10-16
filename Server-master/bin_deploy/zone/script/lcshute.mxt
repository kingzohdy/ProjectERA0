


proc 1

	dialog 1
	add_talk $_npc$:
	add_talk #b#b把忍受变成享受，是精神对于物质的最大胜利，灵魂可以自主，也可以自欺。
		task_add_option 1960 1 100
		task_add_option 1961 1 101
		task_add_option 1962 1 102
		task_add_option 1966 1 103
		task_add_option 2250 1 104
		
		strvar Ret num1
		GET_ITEM_NUM_INPAK 5300932 1 num1
		if num1 > 0
			TASK_STAT 3305 1 Ret
			if Ret = 2
				option 700 曲别针换别墅
			endif
		endif
		
		TASKCYCLE_ADD_OPTION 122 1 150
	talk
	
endproc
proc 150
	TASKCYCLE_DO_OPTION 122 1
endproc 

proc 100
	task_do_option 1960 1
endproc 
proc 101
	task_do_option 1961 1
endproc 
proc 102
	task_do_option 1962 1
endproc 
proc 103
	task_do_option 1966 1
endproc 
proc 104
	task_do_option 2250 1
endproc 

proc 2
	get_trig_task 1 2
	if 2 = 1966
		strvar Mid1 
		get_task_var 1966 0 32 1 Mid1
		
		PLAYER_CALL_BUB 204069 0 Mid1
		set_task_var 1966 0 32 @Mid1 1
		strvar lock 
		var lock 1
		set_task_var 1966 4 32 @lock 1
	endif

endproc

proc 700
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b你想要什么？别墅？哈，别逗了，就你这本破书，顶多换块砖头！
	add_talk #b#b哎~别走啊，我也没说不换是吧。
		option 701 #交换

	talk
	
endproc
proc 701
	
	strvar ItmNum Num Rad
	GET_ITEM_NUM_INPAK 5300932 1 ItmNum
	if ItmNum > 0
			
		GET_PACK_SPACE 0 Num
		if Num > 0
			DEC_ITEM_NUM_INPAK 5300932 1 1
			
		;	rand Rad 100
		;	if @Rad < 99
				ADD_ITEM_NUM_INPAK 5300917 1 0 0
		;	endif
		;	if @Rad = 99
		;		ADD_ITEM_NUM_INPAK 5300934 1 0 0
		;	endif
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