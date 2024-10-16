;302203	戈马利


proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b不打无准备之战。
	add_talk #b#b想要赢得胜利，战前的准备决定了一半胜负。
	
	strvar tian zt1 zt2 zt3 zt4 zt5
	GET_NOW_TIME 0 0 0 tian 0 0 0
	TASK_STAT 3729 0 zt1
	TASK_STAT 3730 0 zt2
	TASK_STAT 3731 0 zt3
	TASK_STAT 3732 0 zt4
	TASK_STAT 3733 0 zt5
	
	if tian = 1 or zt1 = 2 or zt1 = 1
		TASK_ADD_OPTION 3729 0 100
	endif
	if tian = 2 or zt2 = 2 or zt2 = 1
		TASK_ADD_OPTION 3730 0 101
	endif
	if tian = 4 or zt3 = 2 or zt3 = 1
		TASK_ADD_OPTION 3731 0 103
	endif
	if tian = 5 or zt4 = 2 or zt4 = 1
		TASK_ADD_OPTION 3732 0 104
	endif
	
	if ( tian = 3 or tian = 6 or tian = 7 ) or zt5 = 2 or zt5 = 1
		TASK_ADD_OPTION 3733 0 102
	endif
	
	
	talk
endproc

proc 100
	task_do_option 3729 1
endproc

proc 101
	task_do_option 3730 1
endproc

proc 102
	task_do_option 3733 1
endproc

proc 103
	task_do_option 3731 1
endproc

proc 104
	task_do_option 3732 1
endproc

proc 105
	task_do_option 3733 1
endproc

proc 106
	task_do_option 3733 1
endproc
