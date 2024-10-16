;图腾

proc 1
	var 10 0
	SET_MON_VAR 0 32 @10 1
	
	GET_MON_INFO 0 0 50 0 0 0 51 52
	effect 0 0 playEffct($@51$,$@52$,"effect\\entity\\40154_11.ent")

endproc 


proc 2
	add_sys_msg_start
	add_sys_msg 由于你没保护好图腾，任务失败！
	set_sys_msg_mode 3 0 0
	msg_info
	TASK_FAIL 1067 0 
	
endproc 

proc 4
	GET_MON_VAR 0 32 1 10
	add 10 1
	SET_MON_VAR 0 32 @10 1
	
	if 10 = 1
		GET_MON_INFO 0 0 0 0 0 0 20 21
		MON_CALL_ENEMY_MON 235010 2 7 1 @20 @21 500 0 0 0 20 0 0
	endif
endproc 	
