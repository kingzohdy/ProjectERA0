; 端康

proc 1
	dialog 1
	add_talk $_npc$:
	add_talk #b#b我们这片地方，只要开发得当，就会成为人人向往的旅游胜地，总有一天，人们会像潮水一样涌来，为这片土地带来生机……
	
		task_add_option 2108 1 100
		task_add_option 2109 1 101
		task_add_option 2110 1 102
		task_add_option 2135 1 103

	
	talk
endproc

PROC 100
	task_do_option 2108 1
ENDPROC
PROC 101
	task_do_option 2109 1
ENDPROC
PROC 102
	task_do_option 2110 1
ENDPROC
PROC 103
	task_do_option 2135 1
ENDPROC

proc 2
	get_trig_task 1 2
	if 2 = 2110
		task_fini 2110 1
	endif

endproc





