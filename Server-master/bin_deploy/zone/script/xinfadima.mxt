;法蒂玛 
; 携带任务

proc 1
	dialog 1	
	add_talk $_npc$:
	add_talk #b#b恐惧让你沦为囚犯，希望让你重获自由，坚强的人只能救赎自己，伟大的人才能救赎他人！记着，希望是件好东西，没准儿是件最好的东西，而且从没有一样好东西会消逝。
;	task_add_option 1064 1 101	
	task_add_option 1065 1 102

	task_add_option 1066 1 105 
	task_add_option 1068 1 107
	talk
endproc

proc 102
    task_do_option 1065 1
endproc

proc 105
    task_do_option 1066 1
endproc

proc 107
    task_do_option 1068 1
endproc


proc 2
	get_trig_task 1 2
	if 2 = 1068
		task_fini 1068 1
		
	;	NEW_OWNER_TMP_ITEM 1460066 10100 26500 0 0 0 307 1095 1 3600 1
	endif
endproc

