;驯龙者 
; 携带任务\飞龙环游

proc 1

	dialog 1
	add_talk $_npc$:
	add_talk #b#b我们要学会珍惜我们生活的每一天，因为，这每一天的开始，都将是我们余下生命中的第一天。除非我们即将死去。永远不要把事情留到“明天”再做，因为，谁也没法确定，明天，这个世界会发生什么，你，我，他，是否还像现在这般安然……
	task_add_option 1060 1 101	

	task_add_option 1065 1 104
	task_add_option 1064 1 105




	TASK_STAT 1060 1 20
	if 20 = 2 
		OPTION 110 #骑飞龙环游
	endif
	if 20 = 3
		option 111 骑飞龙环游
	
	endif

	talk
endproc
proc 111
	dialog 1
	add_talk $_npc$:
	add_talk #b#b骑飞龙的感觉很好哦！
		option 112 #骑飞龙环游
		option 113 #取消
	talk
endproc

proc 101
    task_do_option 1060 1
endproc

proc 104
    task_do_option 1065 1
endproc
proc 105
    task_do_option 1064 1
endproc

proc 110
	autofly 1 1
	task_fini 1060 41
endproc
proc 112
    	autofly 1 1
endproc

proc 2
	get_trig_task 1 2
	if 2 = 1060
		dialog 1
		add_talk $_npc$:
		add_talk #b#b我们要学会珍惜我们生活的每一天，因为，这每一天的开始，都将是我们余下生命中的第一天。除非我们即将死去。永远不要把事情留到“明天”再做，因为，谁也没法确定，明天，这个世界会发生什么，你，我，他，是否还像现在这般安然……
	
		OPTION 110 #骑飞龙环游
		talk
	endif
	if 2 = 1065
		
	;	NEW_OWNER_TMP_ITEM 1460064 16300 16900 0 0 0 305 1095 1 3600 1
	endif
endproc

