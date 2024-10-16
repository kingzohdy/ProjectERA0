

proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk  #b#b你根本不能明白！我本拥有至高的地位，我本是一族之长，我本是有头有脸足以呼风唤雨的人，而不是一个毫无价值身材矮小无家可归的游民。
	task_add_option 1708 1 11
	task_add_option 1709 1 12
	task_add_option 1710 1 13
	task_add_option 1711 1 14
	task_add_option 1712 1 15
	task_add_option 1713 1 16
	task_add_option 1714 1 17
	task_add_option 1715 1 18
	task_add_option 1716 1 19
	task_add_option 1717 1 20
	task_add_option 1718 1 21
	
	TASK_STAT 3542 1 20
	TASK_STAT 3566 1 21
	TASK_STAT 3612 1 22
	if 20 = 2 or 21 = 2 or 22 = 2
		option 150 公会任务-交出协助物资
	endif
	
	talk
endproc 

PROC 11
	task_do_option 1708 1
ENDPROC
PROC 12
	task_do_option 1709 1
ENDPROC
PROC 13
	task_do_option 1710 1
ENDPROC
PROC 14
	task_do_option 1711 1
ENDPROC
PROC 15
	task_do_option 1712 1
ENDPROC
PROC 16
	task_do_option 1713 1
ENDPROC
PROC 17
	task_do_option 1714 1
ENDPROC
PROC 18
	task_do_option 1715 1
ENDPROC
PROC 19
	task_do_option 1716 1
ENDPROC
PROC 20
	task_do_option 1717 1
ENDPROC
PROC 21
	task_do_option 1718 1
ENDPROC
PROC 150
	
	DIALOG 1
	ADD_TALK $_npc$       
	ADD_TALK #b#b谢谢你们的信任，玛吉族的事情请放心，我也不愿继续看着同伴堕落。
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
		TASK_STAT 3542 1 70
		TASK_STAT 3566 1 71
		TASK_STAT 3612 1 72
		if 70 = 2
			task_fini 3542 1
		endif
		if 71 = 2
			task_fini 3566 1
		endif
		if 72 = 2
			task_fini 3612 1
		endif
	
	endif
ENDPROC


proc 2
	get_trig_task 1 2
	if 2 = 1718 
		task_fini 1718 1
	endif
endproc
