


proc 1

	dialog 1
	
	add_talk $_npc$:
	add_talk #b#b可别掉以轻心，$_player$，虽然这里看起来废弃已久，就是在这座看似荒废了的牢城，许多士兵不幸丧命！
		task_add_option 1963 1 100
		task_add_option 1965 1 101
		
		task_add_option 2260 1 102
		task_add_option 2261 1 103
		
		
		strvar Ret
		task_stat 1965 1 Ret
		if Ret = 2
		;	option #120 领取载具
		
		endif
		
	strvar Ret num1
	GET_ITEM_NUM_INPAK 5300931 1 num1
	if num1 > 0
		TASK_STAT 3305 1 Ret
		if Ret = 2
			option 700 曲别针换别墅
		endif
	endif
	talk
	
endproc


proc 100
	task_do_option 1963 1
endproc 
proc 101
	task_do_option 1965 1
endproc 
proc 102
	task_do_option 2260 1
endproc 
proc 103
	task_do_option 2261 1
endproc 

proc 120	
	APPLY_TMP_ITEM 291003 0 0

endproc


proc 700
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b不想当将军的士兵不是好士兵，我一直梦想能够成为名人，成为将军，成为万人景仰的英雄……做梦无罪，平庸可耻！看在咱们志趣相同的份上，我就帮你一把吧，希望将来也会有人帮我一把！
		option 701 #交换

	talk
	
endproc
proc 701
	
	strvar ItmNum Num Rad
	GET_ITEM_NUM_INPAK 5300931 1 ItmNum
	if ItmNum > 0
			
		GET_PACK_SPACE 0 Num
		if Num > 0
			DEC_ITEM_NUM_INPAK 5300931 1 1
			
		;	rand Rad 100
		;	if @Rad < 95
				ADD_ITEM_NUM_INPAK 5300932 1 0 0
		;	endif
		;	if @Rad >= 95 and @Rad < 99
		;		ADD_ITEM_NUM_INPAK 5300933 1 0 0
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



