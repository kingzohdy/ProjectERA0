;70副本,3号boss 工程师/乘骑（BOSS）
proc 1
	strvar memid
	GET_MON_INFO 1 0 memid 0 0 0 0 0
	TARGET_ADD_STATUS 3 @memid 1095 1 25920000 0 
		
endproc
;死亡事件
proc 2	
	strvar ret defid memid hp mapdef map x y
	GET_MON_INFO ret defid memid hp mapdef map x y
	
	if hp > 0
		return
	endif
	var 10 0
	new_mon_by_dir 205209 1 @x @y 0 0 0 @10 1 2	
endproc
;归位事件
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
