;卢克索



proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b无论是什么人，都会有一套特定的行为、思考方式，时代也是如此，所以，我们很容易能够从一些细节上分辨出这些东西属于哪个时代，也可以根据物品的转变来研究时代的转变。
	
	
	task_add_option 2022 1 100
	task_add_option 2023 1 101
	task_add_option 2024 1 102

	
	
	
	talk
	
endproc 

PROC 100
	task_do_option 2022 1
ENDPROC
PROC 101
	task_do_option 2023 1
ENDPROC
PROC 102
	task_do_option 2024 1
ENDPROC


proc 2
	
	get_trig_task 1 2 
;	if 2 = 2001
;		task_fini 2001 1
;	endif
;	if 2 = 2017
;	
;	endif
endproc

