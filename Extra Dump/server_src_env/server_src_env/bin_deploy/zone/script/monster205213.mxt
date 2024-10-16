;70副本,双面boss
proc 1
	strvar memid
	GET_MON_INFO 1 0 memid 0 0 0 0 0
	TARGET_ADD_STATUS 3 @memid 1095 1 25920000 0
	var 10 0
	set_mon_var 0 32 @10 0
endproc
proc 4
	;boss5flag35,boss5战斗后关门
	var 35 0
	get_pworld_var 100 32 1 35
	if 35 < 1
		is_mon_fighting 1 11
		;boss5flag36,boss5脱离战斗后开门
		var 36 0
		get_pworld_var 104 32 1 36
		if 11 > 0 and 36 < 1
			;boss5flag35,boss5战斗后关门
			var 35 1
			set_pworld_var 100 32 @35 1
		endif
	endif
	
	get_mon_var 0 32 0 10
	if 10 > 0
		return
	endif
	is_mon_fighting 1 11
	if 11 > 0
		strvar memid
		GET_MON_INFO 1 0 memid 0 0 0 0 0
		TARGET_ADD_STATUS 3 @memid 3006 1 30 0 
		var 10 1
		set_mon_var 0 32 @10 0
		;boss5flag35,boss5战斗后关门
		var 35 1
		set_pworld_var 100 32 @35 1
	endif
	
		
endproc
proc 9
	;boss5flag36,boss5脱离战斗后开门
	var 36 1
	set_pworld_var 104 32 @36 1
;	add_sys_msg_start
;	add_sys_msg ookk
;	set_sys_msg_mode 3 0 0
;	map_msg_info
endproc
