;塔依玛脚本
;20--任务1000返回标志 100--
;21--任务1001返回标示 101--任务1001事件 51--任务1001状态
;27--任务1007返回标志 107--
;28--任务1008返回标志 108--任务1008事件 58--任务1008状态
;29--任务1009返回标志 109--任务1009事件 59--任务1009状态
;30--任务1010返回标志 110--任务1010事件	60--任务1010状态
;31--任务1011返回标志 111--任务1011事件
;0--任务未接取 1--任务已完成 2--任务未完成
;600--proc2trig 800--taskid

proc 1

	dialog 1
	
	add_talk 这样宁静的生活,我可不想被那群无知的地人来破坏,我的勇士,你可愿意帮助我？

	task_add_option 1000 20 100
	
	task_add_option 1001 21 101

	task_add_option 1007 27 107
	
	task_add_option 1008 28	108

	task_add_option 1009 29 109

	task_add_option 1010 30 110

	task_add_option 1011 31 111

	talk
endproc

proc 100
	task_do_option 1000 20
endproc

proc 101
	task_do_option 1001 21
endproc

proc 107
	task_do_option 1007 27
endproc

proc 108
	task_do_option 1008 28
endproc

proc 109
	task_do_option 1009 29
endproc

proc 110
	task_do_option 1010 30
endproc

proc 111
	task_do_option 1011 31
endproc

proc 2
	get_trig_task 600 800
	if 800 = 1001
		task_fini 1001 51
	endif
	if 800 = 1011 
		task_fini 1011 61
	endif
endproc
