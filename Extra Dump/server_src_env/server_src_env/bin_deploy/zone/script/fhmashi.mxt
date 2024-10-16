; 玛什

proc 1
	dialog 1
	add_talk $_npc$:
	add_talk #b#b你来的正是时候，#Name#，我们正缺人手。
	add_talk #b#b黑暗议会在这里建议了一座基地，打算利用强大的地心能量，在覆霜要塞中改造出人类的终结者。
	add_talk #b#b我们必须要阻止他们！
		task_add_option 1927 1 100
		task_add_option 1928 1 101
		task_add_option 1929 1 102
		task_add_option 1930 1 103
		task_add_option 1931 1 104
		task_add_option 1932 1 105
	talk
endproc

PROC 100
	task_do_option 1927 1
ENDPROC
PROC 101
	task_do_option 1928 1
ENDPROC
PROC 102
	task_do_option 1929 1
ENDPROC
PROC 103
	task_do_option 1930 1
ENDPROC
PROC 104
	task_do_option 1931 1
ENDPROC
PROC 105
	task_do_option 1932 1
ENDPROC


proc 2
	get_trig_task 1 2
	if 2 = 1931
	;隐身状态
;		strvar Mid
;		GET_PLAYER_ID 1 Mid
;		TARGET_ADD_STATUS 1 Mid 3007 1 30 1
		
	endif
	if 2 = 1932
	;麒麟兽
;		task_fini 1932 1
		NEW_OWNER_TMP_ITEM 1460032 11244 17627 0 0 0 134 1095 1 1000 1
		strvar Mid
		GET_PLAYER_ID 1 Mid
		TARGET_ADD_STATUS 1 @Mid 1033 1 300000 1
	endif
endproc




