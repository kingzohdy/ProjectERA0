


proc 1

	dialog 1
	add_talk $_npc$:
	add_talk #b#b我们费了好大力气才在这座监狱有了立足之地，朋友。我们大部分逃过了亡命之徒的截杀，但我们仍旧失去了很多兄弟。
		task_add_option 1957 1 100
		task_add_option 1958 1 101
		task_add_option 1959 1 102
		
		TASKCYCLE_ADD_OPTION 122 1 150
	talk
	
endproc
proc 150
	TASKCYCLE_DO_OPTION 122 1
endproc 
proc 100
	task_do_option 1957 1
endproc 
proc 101
	task_do_option 1958 1
endproc 
proc 102
	task_do_option 1959 1
endproc 