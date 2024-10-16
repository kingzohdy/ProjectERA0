;梅里耶特拉

proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b曾经，当这里还是一片绿洲的时候，我以为死亡很遥远。直到，看着绿洲一点一点褪去，村民一个个倒下。才开始明白，死亡就在我身边，而我却没办法改变它！
	task_add_option 1502 1 101
;	task_add_option 1503 1 102
	task_add_option 1504 1 103
	task_add_option 1505 1 104
	task_add_option 1506 1 105
;	task_add_option 1507 1 106
	task_add_option 1509 1 108
	
	task_add_option 1598 1 109
	task_add_option 1599 1 110
	
	
	task_add_option 1591 1 111
	task_add_option 1592 1 112
	
;	task_add_option 1550 1 113
;	task_add_option 1560 1 114
;	task_add_option 1585 1 115

	strvar Ret num1
	GET_ITEM_NUM_INPAK 5300917 1 num1
	if num1 > 0
		TASK_STAT 3305 1 Ret
		if Ret = 2
			option 700 曲别针换别墅
		endif
	endif
	
;30~40

	TASK_STAT 3224 1 20
	TASK_STAT 3254 1 21
	TASK_STAT 3516 1 22
	TASK_STAT 3538 1 23
	TASK_STAT 3562 1 24
	TASK_STAT 3608 1 25
	if 20 = 2 or 21 = 2 or 22 = 2 or 23 = 2 or 24 = 2 or 25 = 2
	  	option 150 公会任务-交出协助物资
	endif

	talk
	
endproc 

PROC 101
	task_do_option 1502 1
ENDPROC
;PROC 102
;	task_do_option 1503 1
;ENDPROC
PROC 103
	task_do_option 1504 1
ENDPROC
PROC 104
	task_do_option 1505 1
ENDPROC
PROC 105
	task_do_option 1506 1
ENDPROC

PROC 108
	task_do_option 1509 1
ENDPROC
PROC 109
	task_do_option 1598 1
ENDPROC
PROC 110
	task_do_option 1599 1
ENDPROC
PROC 111
	task_do_option 1591 1
ENDPROC
PROC 112
	task_do_option 1592 1
ENDPROC
;PROC 115
;	task_do_option 1585 1
;ENDPROC



PROC 150	
	DIALOG 1
   	ADD_TALK $_npc$:       
	ADD_TALK  #b#b请允许我代表暖泉镇的居民在此感谢你们的帮助。

	OPTION 250 #交出协助物资
	TALK
ENDPROC
PROC 250
	GET_ITEM_NUM_INPAK 5300533 0 11
	if 11 < 1
		add_sys_msg_start
		add_sys_msg 你身上没有这件物品
		set_sys_msg_mode 1 0 0
		msg_info
	ENDIF
	if 11 >= 1
		dec_item_num_inpak 5300533 1 0
		TASK_STAT 3224 1 70
		TASK_STAT 3254 1 71
		TASK_STAT 3516 1 72
		TASK_STAT 3538 1 73
		TASK_STAT 3562 1 74
		TASK_STAT 3608 1 75
		if 70 = 2
			task_fini 3224 1
		endif
		if 71 = 2
			task_fini 3254 1	
		endif
		if 72 = 2
			task_fini 3516 1	
		endif
		if 73 = 2
			task_fini 3538 1	
		endif
		if 74 = 2
			task_fini 3562 1	
		endif
		if 75 = 2
			task_fini 3608 1	
		endif
	endif
ENDPROC


proc 160
	DIALOG 1
	ADD_TALK $_npc$:       
	ADD_TALK  #b#b新鲜的鲜血已经成为这些不死生物的追求，我们已经陷入恐慌之中了。

	OPTION 260 #(环次)我们一起想办法！
	TALK
endproc
PROC 260
	GET_ITEM_NUM_INPAK 5300514 0 11
	if 11 < 1
		add_sys_msg_start
		add_sys_msg 你身上没有这件物品
		set_sys_msg_mode 4 0 0
		msg_info
	ENDIF
	if 11 >= 1
		dec_item_num_inpak 5300514 1 0
		task_fini 12238 1
	endif
ENDPROC
proc 2
	get_trig_task 1 2
;	if 2 = 1560
;		task_fini 1560 0
;	endif 

endproc


proc 700
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b听说这本书里潜伏着强大的法力，只要有正确的方法，就能够将这力量化为已有；听说这里记录着每一个魔头成长的历史，也许可以从中找到阿拜多斯的弱点。
		option 701 #交换

	talk
	
endproc
proc 701
	
	strvar ItmNum Num Rad
	GET_ITEM_NUM_INPAK 5300917 1 ItmNum
	if ItmNum > 0
			
		GET_PACK_SPACE 0 Num
		if Num > 0
			DEC_ITEM_NUM_INPAK 5300917 1 1
			
		;	rand Rad 100
		;	if @Rad < 80
				ADD_ITEM_NUM_INPAK 5300918 1 0 0
		;	endif
		;	if @Rad >= 80 and @Rad < 95
		;		ADD_ITEM_NUM_INPAK 5300919 1 0 0
		;	endif
		;	if @Rad >= 95 and @Rad < 99
		;		ADD_ITEM_NUM_INPAK 5300920 1 0 0
		;	endif
		;	if @Rad = 99
		;		ADD_ITEM_NUM_INPAK 5300921 1 0 0
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





