;302204	阿道弗


proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b这里的环境遭到黑暗议会的严重破坏和污染。
	add_talk #b#b现在他们似乎又有所行动。
	
	strvar tian zt1 zt2 zt3 zt4 zt5 zt6 zt7
	GET_NOW_TIME 0 0 0 tian 0 0 0
	TASK_STAT 3701 0 zt1
	TASK_STAT 3702 0 zt2
	TASK_STAT 3703 0 zt3
	TASK_STAT 3704 0 zt4
	TASK_STAT 3717 0 zt5
	TASK_STAT 3721 0 zt6
	TASK_STAT 3722 0 zt7
	
	if tian = 1 or zt1 = 2 or zt1 = 1
		TASK_ADD_OPTION 3701 0 100
	endif
	if tian = 2 or zt6 = 2 or zt6 = 1
		TASK_ADD_OPTION 3721 0 101
	endif
	if tian = 3 or zt2 = 2 or zt2 = 1
		TASK_ADD_OPTION 3702 0 102
	endif
	if tian = 4 or zt7 = 2 or zt7 = 1
		TASK_ADD_OPTION 3722 0 103
	endif
	if tian = 5 or zt3 = 2 or zt3 = 1
		TASK_ADD_OPTION 3703 0 104
	endif
	if tian = 6 or zt5 = 2 or zt5 = 1
		TASK_ADD_OPTION 3717 0 105
	endif
	if tian = 7 or zt4 = 2 or zt4 = 1
		TASK_ADD_OPTION 3704 0 106
	endif
	
	talk
endproc

proc 100
	task_do_option 3701 1
endproc

proc 101
	task_do_option 3721 1
endproc

proc 102
	task_do_option 3702 1
endproc

proc 103
	task_do_option 3722 1
endproc

proc 104
	task_do_option 3703 1
endproc

proc 105
	task_do_option 3717 1
endproc

proc 106
	task_do_option 3704 1
endproc
