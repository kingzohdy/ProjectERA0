;70副本,机甲兽怪物
proc 1
	var 10 0
	set_mon_var 0 32 @10 1
endproc
proc 4
	get_mon_var 0 32 1 10 
	if 10 > 0
		is_mon_fighting 1 2
		if 2 < 1
			IS_ONE_MON_FIGHTING_BY_DEFID 1 206150
			if 1 = 0
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
				
				
				
			endif	
		endif
	else
		is_mon_fighting 1 2
		if 2 > 0
			var 10 1
			set_mon_var 0 32 @10 1
		endif
	endif
endproc
