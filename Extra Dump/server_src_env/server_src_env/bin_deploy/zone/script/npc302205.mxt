;302205	古德利安


proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b黑暗议会建起了要塞后，进行一系列人体生化实验。
	add_talk #b#b他们的恶行必将被正义之狮撕碎！
	
	strvar tian zt1 zt2 zt3 zt4 zt5 zt6 zt7
	GET_NOW_TIME 0 0 0 tian 0 0 0
	TASK_STAT 3705 0 zt1
	TASK_STAT 3706 0 zt2
	TASK_STAT 3707 0 zt3
	TASK_STAT 3708 0 zt4
	TASK_STAT 3719 0 zt5
	TASK_STAT 3725 0 zt6
	TASK_STAT 3726 0 zt7
	
	if tian = 1 or zt5 = 2 or zt5 = 1
		TASK_ADD_OPTION 3719 0 100
	endif
	if tian = 2 or zt2 = 2 or zt2 = 1
		TASK_ADD_OPTION 3706 0 101
	endif
	if tian = 3 or zt7 = 2 or zt7 = 1
		TASK_ADD_OPTION 3726 0 102
	endif
	if tian = 4 or zt1 = 2 or zt1 = 1
		TASK_ADD_OPTION 3705 0 103
	endif
	if tian = 5 or zt6 = 2 or zt6 = 1
		TASK_ADD_OPTION 3725 0 104
	endif
	if tian = 6 or zt4 = 2 or zt4 = 1
		TASK_ADD_OPTION 3708 0 105
	endif
	if tian = 7 or zt3 = 2 or zt3 = 1
		TASK_ADD_OPTION 3707 0 106
	endif
	
	talk
endproc

proc 100
	task_do_option 3719 1
endproc

proc 101
	task_do_option 3706 1
endproc

proc 102
	task_do_option 3726 1
endproc

proc 103
	task_do_option 3705 1
endproc

proc 104
	task_do_option 3725 1
endproc

proc 105
	task_do_option 3708 1
endproc

proc 106
	task_do_option 3707 1
endproc
