
proc 1
	strvar Tim
	var Tim 0
	set_task_var 2049 4 8 @Tim 1


endproc 


proc 6
	
	strvar pX pY M Ret Map
	task_stat 2049 1 Ret
	if Ret = 2
	
		get_task_var 2049 4 8 1 Tim
		add Tim 1
		set_task_var 2049 4 8 @Tim 1
		
		if Tim = 3
			GET_PLAYER_MAPINFO 0 Map 0 pX pY
			GET_POINT_DIST 25800 25300 @pX @pY M
			
			if M < 1500 and Map = 3304
				add_sys_msg_start
				add_sys_msg 咦，这是…黄金法老王…再靠近些看个仔细……
				set_sys_msg_mode 3 0 0
				msg_info
				
			endif
			var Tim 0
			set_task_var 2049 4 8 @Tim 1
		endif
	endif
endproc 