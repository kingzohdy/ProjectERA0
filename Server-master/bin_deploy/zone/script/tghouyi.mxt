;;;;新增侯伊



proc 1
	dialog 1

	add_talk $_npc$:
	add_talk #b#b族人的血不能白流，我要为他们报仇雪恨！
	task_add_option 1311 1 100
	task_add_option 1338 1 101
	task_add_option 1339 1 102
	task_add_option 1340 1 103
	task_add_option 1312 1 104
	
	strvar Ret
	task_stat 1312 1 Ret
	if Ret = 2
;		option 300 是否需要我送您一程？
	endif
	talk
endproc 
proc 100
    task_do_option 1311 1
endproc

proc 101
    task_do_option 1338 1
endproc

proc 102
    task_do_option 1339 1
endproc
proc 103
    task_do_option 1340 1
endproc
proc 104
    task_do_option 1312 1
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
	
	NEW_OWNER_TMP_ITEM 1460060 23200 10400 0 0 0 301 1095 1 3600 1
endproc 
proc 302
	return
endproc 
proc 2
	get_trig_task 1 2
	if 2 = 1312
;		dialog 1

;		add_talk $_npc$:
;		add_talk #b#b族人的血不能白流，我要为他们报仇雪恨！
			task_fini 1312 1
;			option 300 是否需要我送您一程？		
;;		;;;协助
	;	talk
	endif

endproc 



