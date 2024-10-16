
proc 1
	dialog 1
	get_role_var 0 1 10 11
	task_stat 4002 22 23
	task_stat 1004 24 25	
	
	add_talk 试练的目的在于为文明挑选真正能够拯救它的勇士。
	task_add_option 1002 12 102
	task_add_option 4002 42 402
	task_add_option 1003 13 103
	task_add_option 1004 14 104
	task_add_option 1005 15 105
	task_add_option 1008 18 108
	task_add_option 1009 19 109
	task_add_option 1010 20 110	
	
	if 11 = 0
		if 23 = 2
			if 25 != 0
				option 123 #返回天空岛	
			endif
		endif
	endif
	
	if 11 = 1
		option 14 #返回天空岛 
	endif
	option 11 #再见
	talk
endproc 

proc 123
	enter_pworld 501 0 1
endproc 

proc 14
	enter_pworld 502 0 1
endproc

proc 102 
	task_do_option 1002 12
endproc

proc 103
	task_do_option 1003 13
endproc

proc 104
	task_do_option 1004 14
endproc

proc 105
	task_do_option 1005 15
endproc

proc 108
	task_do_option 1008 18
endproc

proc 109
	task_do_option 1009 19
endproc

proc 110
	task_do_option 1010 20
endproc

proc 402
	task_do_option 4002 42
endproc

proc 11
	return
endproc

proc 2
	get_trig_task 600 700
	if 700 = 1005
		task_fini 1005 15
	endif
	if 700 = 1010
		task_fini 1010 20
	endif
	if 700 = 1003
		enter_pworld 500 0 1
	endif
	if 700 = 1004
		enter_pworld 501 0 1
	endif
	if 700 = 1009 
		enter_pworld 502 0 1
		set_role_var 0 1 1 10
	endif
endproc

