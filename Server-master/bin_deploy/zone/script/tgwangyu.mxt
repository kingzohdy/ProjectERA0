proc 1

	dialog 1
	add_talk $_npc$:
	add_talk #b#b可怕的阴影终将会消失，就连黑暗也会消失。崭新的一天将会来临，太阳也会散发更明亮的光芒！
;	task_add_option 1311 1 101	
;	task_add_option 1312 1 102	
	task_add_option 1327 1 103
	task_add_option 1401 1 104
	
	task_add_option 3004 1 105
;精灵

	task_add_option 3030 1 106
	
	strvar Loc
	task_stat 3030 1 30
	get_task_var 3030 1 8 1 Loc 

	if 30 = 2 and Loc = 0
		option 280 骑乘拓拓前往时间城
	endif
;	strvar ret
;	task_stat 3004 1 ret
;	if ret = 2
;		option 200 #我想要宠物，请送我过去吧！
;	endif
	talk
endproc
;proc 101
;    task_do_option 1311 1
;endproc
;proc 102
;    task_do_option 1312 1
;endproc
proc 103
    task_do_option 1327 1
endproc
proc 104
    task_do_option 1401 1
endproc
proc 105
   task_do_option 3004 1
endproc

proc 106
    task_do_option 3030 1
endproc
proc 300
	NEW_OWNER_TMP_ITEM 1460049 9800 8800 0 0 0 184 1095 1 3600 1
	strvar Loc
	get_task_var 3030 1 8 Loc 1
	var Loc 1
	set_task_var 3030 1 8 @Loc 1
endproc
proc 301
	return
endproc
proc 2
	get_trig_task 1 2
;	if 2 = 1312 
;		task_fini 1312 1
;	endif

	if 2 = 3110
		task_fini 3110 1
	endif
	if 2 = 1401
		task_fini 1401 1
	endif
	if 2 = 3030
	dialog 1
	add_talk $_npc$:
	add_talk #b#b可怕的阴影终将会消失，就连黑暗也会消失。崭新的一天将会来临，太阳也会散发更明亮的光芒！
		
		option 280 骑乘拓拓前往时间城
		
	talk
	endif
;	if 2 = 3004
;		dialog 1
;		add_talk $_npc$:
;		add_talk #b#b铜鼓的弓箭是魔鬼最害怕的武器！！！
;			option 200 #我想要宠物，请送我过去吧！
;		talk
;		
;	endif
endproc
proc 200
	AUTO_MOVE 20 1000 24200 13200 1460049 0 3081 1 3600
endproc 
proc 280
	dialog 1
	add_talk $_npc$:
	add_talk #b#b拓拓会把你带往时间城的坐骑商人阿诺德处，完成几个简单的任务后，你就会拥有一匹属于自己的#c186506坐骑#n！
	add_talk #b#b同时，你还可以在时间城的“莉莉”处接受#c186506宠物#n系列相关的任务！
	option 300 #爱坐骑、爱宠物，让拓拓送我去时间城吧
	option 301 #先做细语丘陵的任务，等等再去
		
	talk
endproc 

