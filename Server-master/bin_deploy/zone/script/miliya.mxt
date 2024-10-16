

proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b总是有许多人，急切地希望别人知道他都干了些什么，取得了什么丰功伟绩，然后就能获得别人的赞许和嘘声一片。
	
	task_add_option 1560 1 101
	task_add_option 1580 1 102

	
	strvar Ret num1
	GET_ITEM_NUM_INPAK 5300772 1 num1
	if num1 > 0
		TASK_STAT 3304 1 Ret
		if Ret = 2
			option 500 曲别针换别墅
		endif	
	endif
;;;;;;;;;;;;;;;;;;;;;;;;;;;	

	talk
endproc 

PROC 101
	task_do_option 1560 1
ENDPROC
PROC 102
	task_do_option 1580 1
ENDPROC

proc 500
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b我正在寻找净化神殿的办法，或许你手中的这本书中有我要找的东西。
	
	option 501 #交换

	talk
endproc
proc 501
	strvar ItmNum Num Rad
	GET_ITEM_NUM_INPAK 5300772 1 ItmNum
	if ItmNum > 0
			
		GET_PACK_SPACE 0 Num
		if Num > 0
			DEC_ITEM_NUM_INPAK 5300772 1 1
			
		;	rand Rad 100
		;	if @Rad < 80
				ADD_ITEM_NUM_INPAK 5300773 1 0 0
		;	endif
		;	if @Rad >= 80 and @Rad < 95
		;		ADD_ITEM_NUM_INPAK 5300774 1 0 0
		;	endif
		;	if @Rad >= 95 and @Rad < 99
		;		ADD_ITEM_NUM_INPAK 5300775 1 0 0
		;	endif
		;	if @Rad = 99
		;		ADD_ITEM_NUM_INPAK 5300776 1 0 0
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

proc 2
	get_trig_task 1 2
	if 2 = 1580
		task_fini 1580 1
	endif  
endproc
