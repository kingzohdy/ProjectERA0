


proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b站住！哎哎哎…看什么看…说你呢！
	add_talk #b#b叫什么？家住哪？几岁了？家里几口人？老实交待！
	add_talk #b#b银行卡，密码。交出来！
	add_talk #b#b什么？时间城派来的协助者？呃，失礼了，#Name#大人，请勿见怪，最近阳帆港好像是出了内贼，我要对每一个陌生面孔进行排查！

		task_add_option 1366 1 100
		task_add_option 1367 1 101
		task_add_option 1368 1 102

	strvar Ret num1
	GET_ITEM_NUM_INPAK 5300914 1 num1
	if num1 > 0
		TASK_STAT 3305 1 Ret
		if Ret = 2
			option 700 曲别针换别墅
		endif
	endif
	talk
	
endproc 

PROC 100
	task_do_option 1366 1
ENDPROC
PROC 101
	task_do_option 1367 1
ENDPROC
PROC 102
	task_do_option 1368 1
ENDPROC

proc 2

	get_trig_task 1 2
	if 2 = 1368
		task_fini 1368 1
	endif

endproc 
	
proc 700
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b经济这个东西，真是让人捉摸不定，你看那些繁华的码头，即使什么都不做，也会越来越繁华！一旦没落，就一发不可收拾，直至荒无人迹。
	add_talk #b#b从来都是锦上添花容易，可惜我就没有那么好命，所以只能在这近乎荒废的码头上终日做着比时间城还要热闹的美梦。
	add_talk #b#b同样是梦，岂有不成全之理。
		option 701 #交换

	talk
	
endproc
proc 701
	
	strvar ItmNum Num Rad
	GET_ITEM_NUM_INPAK 5300914 1 ItmNum
	if ItmNum > 0
			
		GET_PACK_SPACE 0 Num
		if Num > 0
			DEC_ITEM_NUM_INPAK 5300914 1 1
			
		;	rand Rad 100
		;	if @Rad < 80
				ADD_ITEM_NUM_INPAK 5300915 1 0 0
		;	endif
		;	if @Rad >= 80 and @Rad < 95
		;		ADD_ITEM_NUM_INPAK 5300916 1 0 0
		;	endif
		;	if @Rad >= 95 and @Rad < 99
		;		ADD_ITEM_NUM_INPAK 5300917 1 0 0
		;	endif
		;	if @Rad = 99
		;		ADD_ITEM_NUM_INPAK 5300918 1 0 0
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

