;尤利西斯 
; 携带任务

proc 1
	dialog 1
	add_talk $_npc$:
	add_talk #b#b我觉得生命是最重要的，所以在我心里没有事情是解决不了的。
	add_talk #b#b不是每一个人都可以幸运的过自己理想中的生活，能拥有自己想要的一切固然很好，但如果运气不好恰巧出生在战乱年代，又没有强健的身体，难道就要去死吗？所以呢，我们一定要享受我们所过的生活，并努力向理想的生活靠近，总有一天，理想会变成现实。
	task_add_option 1069 1 101	
;	task_add_option 1071 1 102
	task_add_option 1072 1 103
	task_add_option 1073 1 104
	task_add_option 1078 1 105
	task_add_option 1079 1 106

	talk
endproc
proc 101
    task_do_option 1069 1
endproc

proc 103
    task_do_option 1072 1
endproc
proc 104
    task_do_option 1073 1
endproc
proc 105
    task_do_option 1078 1
endproc
proc 106
    task_do_option 1079 1
endproc

proc 2
	get_trig_task 1 2
	if 2 = 1073
		effect 0 0 UseTaskItemToMonsterLimit(1073, 99, 250001)
	endif
	if 2 = 1079
		task_fini 1079 1
		
		autofly 121 1
	endif

endproc 
