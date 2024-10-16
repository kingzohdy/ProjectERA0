

proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b人民卫队只一个目标：保护落日荒野的安宁，还这片土地富饶与和平。不幸的是，和平总是以鲜血为代价来换取的。
	
;40~50
	task_add_option 1602 1 127
	task_add_option 1603 1 128
	task_add_option 1605 1 129
	task_add_option 1606 1 130
	task_add_option 1607 1 131
	task_add_option 1618 1 132
	task_add_option 1625 1 134
	
	task_add_option 1651 1 135
	
	task_add_option 1701 1 136
	task_add_option 1629 1 137
	task_add_option 1630 1 138
	task_add_option 1631 1 139
	
	TASK_STAT 3519 1 20
	TASK_STAT 3541 1 21
	TASK_STAT 3565 1 22
	TASK_STAT 3611 1 23
	if 20 = 2 or 21 = 2 or 22 = 2 or 23 = 2
		option 150 公会任务-交出协助物资
	endif

	talk
endproc 


PROC 127
	task_do_option 1602 1
ENDPROC
PROC 128
	task_do_option 1603 1
ENDPROC
PROC 129
	task_do_option 1605 1
ENDPROC
PROC 130
	task_do_option 1606 1
ENDPROC
PROC 131
	task_do_option 1607 1
ENDPROC
PROC 132
	task_do_option 1618 1
ENDPROC

PROC 134
	task_do_option 1625 1
ENDPROC
PROC 135
	task_do_option 1651 1
ENDPROC

PROC 136
	task_do_option 1701 1
ENDPROC
PROC 137
	task_do_option 1629 1
ENDPROC
PROC 138
	task_do_option 1630 1
ENDPROC
PROC 139
	task_do_option 1631 1
ENDPROC
PROC 150
	
	DIALOG 1
	ADD_TALK $_npc$       
	ADD_TALK #b#b有了公会资助的物资，居民们的士气也大大的提高了。
	OPTION 250 #交出协助物资
	TALK
ENDPROC

PROC 250
	
	GET_ITEM_NUM_INPAK 5300533 0 11
	if 11 < 1
		add_sys_msg_start
		add_sys_msg 你身上没有这件物品！
		set_sys_msg_mode 1 0 0
		msg_info
	ENDIF
	if 11 >= 1
		dec_item_num_inpak 5300533 1 0
		TASK_STAT 3519 1 70
		TASK_STAT 3541 1 71
		TASK_STAT 3565 1 72
		TASK_STAT 3611 1 73
		if 70 = 2
			task_fini 3519 1
		endif
		if 71 = 2
			task_fini 3541 1
		endif
		if 72 = 2
			task_fini 3565 1
		endif
		if 73 = 2
			task_fini 3611 1
		endif
	
	endif
ENDPROC

proc 2
	get_trig_task 1 2
	if 2 = 1651 
		task_fini 1651 1
	endif
	if 2 = 1701 
		task_fini 1701 1
	endif
endproc
