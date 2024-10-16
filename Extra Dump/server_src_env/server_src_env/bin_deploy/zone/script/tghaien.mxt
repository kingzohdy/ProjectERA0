proc 1
	dialog 1
	add_talk $_npc$:
	add_talk #b#b小心，这周围都是吃人的鳄鱼！
	task_add_option 1324 1 101
	task_add_option 1325 1 102
	task_add_option 1330 1 103
	task_add_option 1331 1 104
	task_add_option 1332 1 105
	task_add_option 1333 1 106
	strvar Ret
	task_stat 1325 1 Ret
	if Ret = 2
;		option 300 是否需要我送您一程？
	endif
	talk
endproc

proc 101
    task_do_option 1324 1
endproc

proc 102
    task_do_option 1325 1
endproc
proc 103
    task_do_option 1330 1
endproc
proc 104
    task_do_option 1331 1
endproc
proc 105
    task_do_option 1332 1
endproc
proc 106
    task_do_option 1333 1
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
	
	NEW_OWNER_TMP_ITEM 1460063 12900 21000 0 0 0 304 1095 1 3600 1
endproc 
proc 302
	return
endproc 
proc 2
	get_trig_task 1 2
	if 2 = 1325
;		dialog 1
;		add_talk $_npc$:
;		add_talk #b#b小心，这周围都是吃人的鳄鱼！
			task_fini 1325 1
;			option 300 是否需要我送您一程？		
;		talk
	endif

endproc
