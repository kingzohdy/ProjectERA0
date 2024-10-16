
proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b我的家族世代相传皆是考古专家，不过你看上去倒是蛮机灵的，替未来的考古大师跑跑腿怎么样？
		
		task_add_option 2040 1 100
		task_add_option 2041 1 101
		task_add_option 2042 1 102

	talk
endproc 

proc 100
	task_do_option 2040 1
endproc 
proc 101
	task_do_option 2041 1
endproc 
proc 102
	task_do_option 2042 1
endproc 

