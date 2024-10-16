;天空城教官脚本
;39--任务1019返回标志 119--
;40--任务1020返回标志 120--任务1020事件 70--任务1020状态
;0--任务未接取 1--任务已完成 2--任务未完成
;600--proc2trig 800--taskid

proc 1

	dialog 1
	
	add_talk 在战场上活下来才是最大的本事,天空城,能够活下来的人才会被认同,才是真正的勇士。

	task_add_option 1019 39 119
	
	task_add_option 1020 40 120
	
	option 107 #重置副本
	
	talk
endproc

proc 119
	task_do_option 1019 39
endproc

proc 120
	task_do_option 1020 40
endproc

proc 107
	reset_pworld 502
	ADD_SYS_MSG_START
	ADD_SYS_MSG 副本己重置
	SET_SYS_MSG_MODE 4 0 0
	MAP_MSG_INFO
endproc

proc 2
	GET_TRIG_TASK 600 800
	IF 800 = 1020
		ENTER_PWORLD 502 0 1		
	ENDIF
ENDPROC
