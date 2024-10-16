

proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b我喜欢有胆识有知识并且聪明的人。他们不惧怕危险，能够应付一切突发状况，并且懂得权衡利弊，不会做出一些损人不利已的事情。当然，最关键的是，聪明的人往往活的更长久！这世界上没有什么比培养一个得力助手更艰难的事了！
	add_talk #b#b
	add_talk #c186506注意：环组任务放弃后算作完成一次。
	add_talk #c186506注意：接取任务后，将不能使用“我很忙”将其立刻完成。#n
	
	task_add_option 1651 1 101
	task_add_option 1652 1 102
	task_add_option 1653 1 103
	task_add_option 1654 1 104
	task_add_option 1655 1 105
	task_add_option 1660 1 106
	TASKCYCLE_ADD_OPTION 106 1 107
	
	
	task_add_option 1662 1 108
	task_add_option 1663 1 109
	task_add_option 1629 1 110
	task_add_option 3000 1 111
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

	strvar lv
	GET_PLAYER_BASE_ATT 0 0 lv 1

	if lv >= 45 and lv < 55
		option 220 我要立刻完成【环组】十三号矿洞
	endif
	talk
	
endproc 

PROC 101
	task_do_option 1651 1
ENDPROC
PROC 102
	task_do_option 1652 1
ENDPROC
PROC 103
	task_do_option 1653 1
ENDPROC
PROC 104
	task_do_option 1654 1
ENDPROC
PROC 105
	task_do_option 1655 1
ENDPROC
PROC 106
	task_do_option 1660 1
ENDPROC
PROC 107
	TASKCYCLE_DO_OPTION 106 1
ENDPROC

PROC 108
	task_do_option 1662 1
ENDPROC
PROC 109
	task_do_option 1663 1
ENDPROC
PROC 110
	task_do_option 1629 1
ENDPROC
PROC 111
	task_do_option 3000 1
ENDPROC


proc 2
	get_trig_task 1 2
	if @2 = 1655
		task_fini 1655 1
	endif    
	if 2 = 1663
		
		add_item_num_inpak 5300862 5 1 0	
	endif
	if 2 = 3000
		task_fini 3000 1
	endif
endproc
proc 220
	strvar lv
	GET_PLAYER_BASE_ATT 0 0 lv 1
	if lv >= 45 and lv < 55
		PLAYER_BUSY_UI 17 1
	endif
endproc 