
proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b一个接一个……我们的敌人都将被消灭！一个接一个，他们都将走向毁灭！
		task_add_option 2053 1 100
		task_add_option 2054 1 101

	talk
endproc 

proc 100
	task_do_option 2053 1
endproc 
proc 101
	task_do_option 2054 1
endproc 

