

proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b战斗的感觉很消耗体力吧，是不是考虑做点别的？
	task_add_option 4013 1 101
	task_add_option 4014 1 102
		OPTION 110 #蒸汽科技

	talk
endproc 

PROC 110
	OPEN_UI 43
ENDPROC

PROC 101
	task_do_option 4013 1
ENDPROC
PROC 102
	task_do_option 4014 1
ENDPROC


proc 2
	get_trig_task 1 2
	if 2 = 4013
		task_fini 4013 1
	endif
	if 2 = 4014
		task_fini 4014 1
	endif

endproc




