

proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b生活中无论有什么闪失，统统是自己的错，与人无尤，从错处学习改过，精益求精，直至不犯相同的错误，不要把过失推诿到他人肩膀上去，免得失去学乖的机会！
	TASKCYCLE_ADD_OPTION 106 1 101

	
		task_add_option	2247 1 103
	talk
endproc 


PROC 101
	TASKCYCLE_DO_OPTION 106 1
ENDPROC

PROC 103
	task_do_option 2247 1
ENDPROC
