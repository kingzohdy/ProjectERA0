proc 1
	dialog 1
	add_talk $_npc$:
	add_talk #b#b铜鼓村是天神赐予给我们的，我要竭尽所能去保卫它。
	task_add_option 1304 1 101
	task_add_option 1305 1 102
	task_add_option 1306 1 103 

	strvar Ret
	task_stat 1306 1 Ret
	if Ret = 2
;		option 300 是否需要我送您一程？
	endif

	talk
endproc

proc 101
    task_do_option 1304 1
endproc
proc 102
    task_do_option 1305 1
endproc
proc 103
    task_do_option 1306 1
endproc
proc 300
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b我可以直接将您送到目的地，但是中途不会停哟！
	
		option 301 #请直接将我送到目的地吧！
		option 302 #不用啦，我自己过去！
	talk

endproc 
proc 301
	
	NEW_OWNER_TMP_ITEM 1460059 8700 8400 0 0 0 300 1095 1 3600 10
endproc 
proc 302
	return
endproc 
proc 2
	get_trig_task 1 600

	if 600 = 1306
;		dialog 1
;		add_talk $_npc$:
;		add_talk #b#b铜鼓村是天神赐予给我们的，我要竭尽所能去保卫它。
		task_fini 1306 1
;		option 300 是否需要我送您一程？
;		talk
		;协助
	
	endif

endproc 
