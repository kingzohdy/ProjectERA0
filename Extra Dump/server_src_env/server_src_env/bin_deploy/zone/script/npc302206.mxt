;302206	尼弥兹


proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b这里蕴藏着丰富的地底资源，因此吸引了黑暗议会到来。
	add_talk #b#b我们必须阻止他们肆掠资源。
	
	strvar tian zt1 zt2 zt3 zt4 zt5 zt6 zt7
	GET_NOW_TIME 0 0 0 tian 0 0 0
	TASK_STAT 3709 0 zt1
	TASK_STAT 3710 0 zt2
	TASK_STAT 3711 0 zt3
	TASK_STAT 3712 0 zt4
	TASK_STAT 3718 0 zt5
	TASK_STAT 3723 0 zt6
	TASK_STAT 3724 0 zt7
	
	if tian = 1 or zt2 = 2 or zt2 = 1
		TASK_ADD_OPTION 3710 0 100
	endif
	if tian = 2 or zt4 = 2 or zt4 = 1
		TASK_ADD_OPTION 3712 0 101
	endif
	if tian = 3 or zt7 = 2 or zt7 = 1
		TASK_ADD_OPTION 3724 0 102
	endif
	if tian = 4 or zt3 = 2 or zt3 = 1
		TASK_ADD_OPTION 3711 0 103
	endif
	if tian = 5 or zt6 = 2 or zt6 = 1
		TASK_ADD_OPTION 3723 0 104
	endif
	if tian = 6 or zt1 = 2 or zt1 = 1
		TASK_ADD_OPTION 3709 0 105
	endif
	if tian = 7 or zt5 = 2 or zt5 = 1
		TASK_ADD_OPTION 3718 0 106
	endif
	
	talk
endproc

proc 100
	task_do_option 3710 1
endproc

proc 101
	task_do_option 3712 1
endproc

proc 102
	task_do_option 3724 1
endproc

proc 103
	task_do_option 3711 1
endproc

proc 104
	task_do_option 3723 1
endproc

proc 105
	task_do_option 3709 1
endproc

proc 106
	task_do_option 3718 1
endproc
