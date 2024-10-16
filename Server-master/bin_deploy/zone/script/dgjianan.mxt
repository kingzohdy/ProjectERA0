

proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b这世界上的东西，管它是看得见的，还是看不见的，管它是死人的还是活人的，只要有人敢买，并且价格合适，我就敢卖。
	add_talk #b#b
	add_talk #c186506注意：环组任务放弃后算作完成一次。
	add_talk #c186506注意：接取任务后，将不能使用“我很忙”将其立刻完成。#n
	
	task_add_option 1534 1 101
	task_add_option 1533 1 102
	task_add_option 1560 1 103
;	task_add_option 1573 1 104
;	task_add_option 1574 1 105
;
;	task_add_option 1575 1 106
;	task_add_option 1576 1 107 

	TASKCYCLE_ADD_OPTION 103 1 140
	
	strvar lv
	GET_PLAYER_BASE_ATT 0 0 lv 1
	
	if lv >= 35 and lv < 45
		option 210 我要立刻完成【环组】阿努比斯神殿
	endif

	talk
endproc 
proc 140
	TASKCYCLE_DO_OPTION 103 1
endproc

PROC 101
	task_do_option 1534 1
ENDPROC
PROC 102
	task_do_option 1533 1
ENDPROC
PROC 103
	task_do_option 1560 1
ENDPROC

proc 2
	get_trig_task 1 2
	if 2 = 1560
		task_fini 1560 0
	endif 

endproc
proc 210
	strvar lv
	GET_PLAYER_BASE_ATT 0 0 lv 1
	if lv >= 35 and lv < 45
		PLAYER_BUSY_UI 16 1
	endif
endproc



