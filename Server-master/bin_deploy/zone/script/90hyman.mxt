; 爷们


proc 1
	dialog 1
	add_talk $_npc$:
	add_talk #b#b我了个去，这是什么世道，外面居然比家里还安全。
	
		task_add_option 2119 1 100
		task_add_option 2120 1 101
		task_add_option 2121 1 102
		
		strvar Ret
		task_stat 2121 1 Ret
		if Ret = 2
			option 150 #我准备好了

		endif
	talk
endproc

PROC 100
	task_do_option 2119 1
ENDPROC
PROC 101
	task_do_option 2120 1
ENDPROC
PROC 102
	task_do_option 2121 1
ENDPROC



proc 2
	get_trig_task 1 2
	if 2 = 2121
		dialog 1
		add_talk $_npc$:
		add_talk #b#b我了个去，这是什么世道，外面居然比家里还安全。
		
			option 150 #我准备好了
		talk
	endif
endproc
proc 150
	strvar x y mid
	get_task_var 2121 0 32 1 mid	
	DEL_ID_MON @Mid 1
	
	PLAYER_CALL_BUB 254024 180 10
	
	get_task_var 2121 0 32 1 mid
	var mid @10
	set_task_var 2121 0 32 @mid 1
;	CALL_MON_BY_PATROL 1 254023 0 0 32000 29200 503 0 Mid

	add_sys_msg_start
	add_sys_msg 目标：桑丝村！出发！
	set_sys_msg_mode 3 0 0
	msg_info
	
	return
endproc 





