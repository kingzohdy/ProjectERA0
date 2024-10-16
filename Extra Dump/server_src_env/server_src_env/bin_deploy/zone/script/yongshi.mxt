;丛林勇士
proc 1

	dialog 1
	add_talk 详细的情况我会好好同长官交代的，不过真的感谢你的帮助。
	task_add_option 4030 430 30
	talk
	
endproc

proc 30
	
	task_do_option 4030 430
	
endproc
