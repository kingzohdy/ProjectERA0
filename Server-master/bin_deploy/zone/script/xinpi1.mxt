;竞技场教官脚本
;22--任务1002返回标志 102--             52--任务1002状态
;23--任务1003返回标志 103--任务1003事件 53--任务1003状态
;24--任务1004返回标志 104--任务1004事件 54--任务1004状态

;0--任务未接取 1--任务已完成 2--任务未完成
;600--proc2trig 800--taskid
;501--进入副本1 502--离开 

proc 1

	dialog 1
	
	add_talk 竞技场的血腥味,大概这群所谓的“勇士”会很不喜欢，不过这个还远不是战场的残酷。

	task_add_option 1002 22 102
	
	task_add_option 1003 23 103

	task_add_option 1004 24 104
	
	task_stat 1003 23 53
	if 53 = 3
		option 501 #进入副本
	endif
	
	if 53 = 2
		option 503 #进入副本
	endif
	
	option 502 #离开

	option 107 #重置副本
	talk
endproc

proc 102
	task_do_option 1002 22
endproc

proc 103
	task_do_option 1003 23
endproc

proc 104
	task_do_option 1004 24
endproc

proc 107
	reset_pworld 500
	ADD_SYS_MSG_START
	ADD_SYS_MSG 副本己重置
	SET_SYS_MSG_MODE 4 0 0
	MAP_MSG_INFO
endproc

proc 2	
	get_trig_task 600 800
	if 800 = 1004
		task_fini 1004 24
	endif
	if 800 = 1003
		enter_pworld 500 0 1
	endif
endproc

proc 501 
	enter_pworld 500 0 1
endproc

proc 502
	return
endproc

