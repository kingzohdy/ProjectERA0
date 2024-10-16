proc 1
	dialog 1
	add_talk 新手给老子过来!!
	task_add_option 1204 24 124
	talk
endproc 

proc 124
	task_do_option 1204 24
endproc 