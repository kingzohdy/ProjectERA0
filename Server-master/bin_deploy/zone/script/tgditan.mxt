proc 1
	dialog 1
	add_talk $_npc$:
	add_talk #b#b人是铁，饭是钢。年轻人，来我的摊子上看看嘛，有没有你需要的东西。
;	task_add_option 1337 1 101
;	task_add_option 1338 1 102
;;	task_add_option 1339 1 103	
;	task_add_option 1340 1 104	
	
	task_add_option 1341 1 112
	task_add_option 1342 1 113
	task_add_option 1343 1 114
	task_add_option 1344 1 115
	task_add_option 1345 1 116
	task_add_option 1346 1 117
	task_add_option 1347 1 118
	task_add_option 1348 1 119
	task_add_option 1349 1 120
	task_add_option 1350 1 121
	task_add_option 1351 1 122
	task_add_option 1352 1 123

	
	OPTION 110 #打开商店 
	talk
endproc

;proc 101
;	task_do_option 1337 1
;endproc
;proc 102
;	task_do_option 1338 1
;endproc
;proc 103
;	task_do_option 1339 1
;endproc
;proc 104
;	task_do_option 1340 1
;endproc

proc 112
    task_do_option 1341 1
endproc
proc 113
    task_do_option 1342 1
endproc
proc 114
    task_do_option 1343 1
endproc
proc 115
    task_do_option 1344 1
endproc
proc 116
    task_do_option 1345 1
endproc
proc 117
    task_do_option 1346 1
endproc
proc 118
    task_do_option 1347 1
endproc
proc 119
    task_do_option 1348 1
endproc
proc 120
    task_do_option 1349 1
endproc
proc 121
    task_do_option 1350 1
endproc
proc 122
    task_do_option 1351 1
endproc
proc 123
    task_do_option 1352 1
endproc


proc 110
    OPEN_NPC_SHOP
endproc


;proc 2
;	get_trig_task 1 600
;	if 600 = 1084
;		task_fini 1084 1
;	endif
;ENDPROC
