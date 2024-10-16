;竞技场教官 
; 携带任务\进入副本500

proc 1
	TASK_STAT 1054 1 3
	dialog 1
	add_talk $_npc$:
	add_talk #b#b我要把有限的生命，投入到无限的为自治联盟服务中去！训练出更多强大的精英士兵，保卫人民的安全！
	task_add_option 1052 1 101	
	task_add_option 1054 1 102
	task_add_option 1055 1 103
	task_add_option 1057 1 104

	
	if 3 = 2
		OPTION 110 #进入试炼场
	endif
	
	talk
endproc
proc 101
    task_do_option 1052 1
endproc
proc 102
    task_do_option 1054 1
endproc
proc 103
    task_do_option 1055 1
endproc
proc 104
    task_do_option 1057 1
endproc
proc 110
    enter_pworld 500 0 1
endproc


proc 2
	get_trig_task 1 2

	if 2 = 1054 
		dialog 1
		add_talk $_npc$:
		add_talk #b#b我要把有限的生命，投入到无限的为自治联盟服务中去！训练出更多强大的精英士兵，保卫人民的安全！
	
		OPTION 110 #进入试炼场
		talk
	endif

endproc

