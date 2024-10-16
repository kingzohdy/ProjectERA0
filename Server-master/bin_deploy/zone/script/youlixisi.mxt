;尤利西斯脚本
;35--任务1015返回标志 115--
;36--任务1016返回标志 116--任务1016事件 66--任务1016状态
;37--任务1017返回标志 117--任务1017事件 67--任务1016状态
;38--任务1018返回标志 118--任务1018事件 68--任务1018状态
;0--任务未接取 1--任务已完成 2--任务未完成
;600--proc2trig 800--taskid

proc 1
	dialog 1
	
	add_talk 从祖辈就开始的战争,到现在不断的战火,我已经老了,也许看不到胜利那天的到来。

	task_add_option 1015 35 115
	
	task_add_option 1016 36 116

	task_add_option 1017 37 117

	task_add_option 1018 38 118

	talk
endproc 

proc 115
	task_do_option 1015 35
endproc

proc 116
	task_do_option 1016 36
endproc

proc 117
	task_do_option 1017 37
endproc

proc 118
	task_do_option 1018 38
endproc

proc 2
	get_trig_task 600 800
	if 800 = 1018
		task_fini 1018 68
	endif
endproc
