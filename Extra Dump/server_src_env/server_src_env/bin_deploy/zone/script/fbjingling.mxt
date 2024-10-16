;精灵商人 316002


proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b我这里有各种精灵的魂哦，都是上等的品种！
	
	task_add_option 3111 1 100
	task_add_option 3113 1 101
	task_add_option 3114 1 102

	
	talk
endproc 


proc 100
	task_do_option 3111 1
endproc
proc 101
	task_do_option 3113 1
endproc
proc 102
	task_do_option 3114 1
endproc
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

proc 2
	get_trig_task 1 2 
	if 2 = 3114
		strvar Num_1
		get_pworld_var 6 8 1 Num_1
		var Num_1 0
		set_pworld_var 6 8 @Num_1 1
	
	endif

endproc 

