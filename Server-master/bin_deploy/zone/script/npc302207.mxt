;302207	温司顿


proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b黑暗议会在这里进行了大量人体生化实验。
	add_talk #b#b现在他们似乎正暗中监视着我们。
	
	strvar tian zt1 zt2 zt3 zt4 zt5 zt6 zt7
	GET_NOW_TIME 0 0 0 tian 0 0 0
	TASK_STAT 3713 0 zt1
	TASK_STAT 3714 0 zt2
	TASK_STAT 3715 0 zt3
	TASK_STAT 3716 0 zt4
	TASK_STAT 3720 0 zt5
	TASK_STAT 3727 0 zt6
	TASK_STAT 3728 0 zt7
	
	if tian = 1 or zt3 = 2 or zt3 = 1
		TASK_ADD_OPTION 3715 0 100
	endif
	if tian = 2 or zt6 = 2 or zt6 = 1
		TASK_ADD_OPTION 3727 0 101
	endif
	if tian = 3 or zt4 = 2 or zt4 = 1
		TASK_ADD_OPTION 3716 0 102
	endif
	if tian = 4 or zt7 = 2 or zt7 = 1
		TASK_ADD_OPTION 3728 0 103
	endif
	if tian = 5 or zt2 = 2 or zt2 = 1
		TASK_ADD_OPTION 3714 0 104
	endif
	if tian = 6 or zt5 = 2 or zt5 = 1
		TASK_ADD_OPTION 3720 0 105
	endif
	if tian = 7 or zt1 = 2 or zt1 = 1
		TASK_ADD_OPTION 3713 0 106
	endif
	
	talk
endproc

proc 100
	task_do_option 3715 1
endproc

proc 101
	task_do_option 3727 1
endproc

proc 102
	task_do_option 3716 1
endproc

proc 103
	task_do_option 3728 1
endproc

proc 104
	task_do_option 3714 1
endproc

proc 105
	task_do_option 3720 1
endproc

proc 106
	task_do_option 3713 1
endproc
