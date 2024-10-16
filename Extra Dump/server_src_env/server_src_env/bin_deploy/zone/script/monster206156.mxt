;70副本,70副本boss3
proc 1
	strvar memid
	GET_MON_INFO 1 0 memid 0 0 0 0 0
	TARGET_ADD_STATUS 3 @memid 1095 1 25920000 0 
		
endproc
proc 9
	;boss3flagclose38,boss3脱离战斗后开门
	var 38 0
	get_pworld_var 112 32 1 38
	if 38 = 0 
		var 38 1
		;boss3flagclose38,boss3脱离战斗后开门
		
		set_pworld_var 112 32 @38 1
	;	add_sys_msg_start
	;	add_sys_msg 123
	;	set_sys_msg_mode 3 0 0
	;	map_msg_info
	endif
	
	
	
endproc
