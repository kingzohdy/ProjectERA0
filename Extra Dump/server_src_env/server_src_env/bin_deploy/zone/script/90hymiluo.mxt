; 米罗


proc 1
	dialog 1
	add_talk $_npc$:
	add_talk #b#b既然老天让我逃过了这场劫难，说明他想留下我这有用之躯为这世界做点什么，他一定也不愿看到他最爱的子民变成如此这副模样。
	
		task_add_option 2121 1 100
		task_add_option 2122 1 101
		task_add_option 2130 1 102
		task_add_option 2131 1 103
		
	
	talk
endproc

PROC 100
	task_do_option 2121 1
ENDPROC
PROC 101
	task_do_option 2122 1
ENDPROC
PROC 102
	task_do_option 2130 1
ENDPROC
PROC 103
	task_do_option 2131 1
ENDPROC
proc 2
	get_trig_task 1 2
	if 2 = 2122
		task_fini 2122 1
	endif
endproc






