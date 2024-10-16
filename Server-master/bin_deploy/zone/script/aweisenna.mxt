

proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b这里曾是一片绿洲…但我从出生起，这里就是看到的这幅模样，那边的河流能滋润到的地方也越来越少了…我多么希望看见一片绿色，动物成群…
	
	task_add_option 1522 1 101
	task_add_option 1523 1 102
	task_add_option 1524 1 103
	task_add_option 1525 1 104
	task_add_option 1526 1 105
	task_add_option 1527 1 106
	task_add_option 1528 1 107

	strvar Ret num1
	GET_ITEM_NUM_INPAK 5300771 1 num1
	if num1 > 0
		TASK_STAT 3304 1 Ret
		if Ret = 2
			option 500 曲别针换别墅
		endif	
	endif
	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

	talk
endproc 
proc 500
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b我已经收集了七七四十九种历险故事了，偏偏没有你手中这一本，我愿意与你交换了。
	
	option 501 #交换

	talk
endproc
proc 501
	strvar ItmNum Num Rad
	GET_ITEM_NUM_INPAK 5300771 1 ItmNum
	if ItmNum > 0
			
		GET_PACK_SPACE 0 Num
		if Num > 0
			DEC_ITEM_NUM_INPAK 5300771 1 1
			
		;	rand Rad 100
		;	if @Rad < 80
				ADD_ITEM_NUM_INPAK 5300772 1 0 0
		;	endif
		;	if @Rad >= 80 and @Rad < 95
		;		ADD_ITEM_NUM_INPAK 5300773 1 0 0
		;	endif
		;	if @Rad >= 95 and @Rad < 99
		;		ADD_ITEM_NUM_INPAK 5300774 1 0 0
		;	endif
		;	if @Rad = 99
		;		ADD_ITEM_NUM_INPAK 5300775 1 0 0
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

PROC 101
	task_do_option 1522 1
ENDPROC
PROC 102
	task_do_option 1523 1
ENDPROC
PROC 103
	task_do_option 1524 1
ENDPROC
PROC 104
	task_do_option 1525 1
ENDPROC
PROC 105
	task_do_option 1526 1
ENDPROC

PROC 106
	task_do_option 1527 1
ENDPROC
PROC 107
	task_do_option 1528 1
ENDPROC


proc 2
	get_trig_task 1 2

	if 2 = 1528
		task_fini 1528 1
	endif 
	if 2 = 1525 
		
		ADD_ITEM_NUM_INPAK 5300351 1 1 0
		ADD_ITEM_NUM_INPAK 5300352 6 1 0
	endif


endproc
