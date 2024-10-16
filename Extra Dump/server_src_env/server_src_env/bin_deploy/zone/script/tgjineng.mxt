;技能学习员

proc 1
	dialog 1
	
	add_talk $_npc$:
	add_talk #b#b前进，无畏黑暗！耶提的勇士！起来，起来，耶提的勇士！冲向毁灭，冲向黑暗，长矛震撼，盾牌粉碎……直到太阳升起！
	
	task_add_option 1319 1 101	
	task_add_option 1320 1 102
	task_add_option 1321 1 103
	task_add_option 1322 1 104

	OPTION 150 #学习技能
		
	talk
endproc

proc 101
    task_do_option 1319 1
endproc
proc 102
    task_do_option 1320 1
endproc
proc 103
    task_do_option 1321 1
endproc
proc 104
    task_do_option 1322 1
endproc


proc 150
    OPEN_UI 1
endproc
proc 2
	get_trig_task 1 600
	if 600 = 1322
			ADD_ITEM_NUM_INPAK 5300388 1 1 0
			ADD_ITEM_NUM_INPAK 5300390 1 1 0
			ADD_ITEM_NUM_INPAK 5300391 1 1 0
	endif
ENDPROC














