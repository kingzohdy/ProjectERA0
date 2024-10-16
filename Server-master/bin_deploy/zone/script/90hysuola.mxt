; 皮克希

proc 1
	dialog 1
	add_talk $_npc$:
	add_talk #b#b在我梦中，所有人都唱着同一首爱的歌，蓝天碧草之间笑声成了世界通用的语言，然而当我醒来的时候发现，世界上到处都是需要帮助的人……是不是我错过了什么？有那么多需要帮助的人，却无人伸出援助之手……
	
		task_add_option 2101 1 100
		task_add_option 2102 1 101
		task_add_option 2103 1 102
		task_add_option 2104 1 103
		task_add_option 2105 1 104

	talk
endproc

PROC 100
	task_do_option 2101 1
ENDPROC
PROC 101
	task_do_option 2102 1
ENDPROC
PROC 102
	task_do_option 2103 1
ENDPROC

PROC 103
	task_do_option 2104 1
ENDPROC

PROC 104
	task_do_option 2105 1
ENDPROC



proc 2
	get_trig_task 1 2
	if 2 = 2104
		
		ADD_ITEM_NUM_INPAK 5300891 1 1 0
		ADD_ITEM_NUM_INPAK 5300892 1 1 0
	endif
	if 2 = 2105
		task_fini 2105 1
	endif

endproc
