
proc 1
	strvar Tim
	var Tim 0
	set_task_var 2041 4 8 @Tim 1


endproc 


proc 6
	
	strvar pX pY M Ret
	task_stat 2041 1 Ret
	if Ret = 2
		get_task_var 2041 4 8 1 Tim
		add Tim 1
		set_task_var 2041 4 8 @Tim 1
		
		GET_PLAYER_MAPINFO 0 0 0 pX pY
		GET_POINT_DIST 25700 44900 @pX @pY M	
		
		if Tim = 3
			if M < 1000
			
				add_sys_msg_start
				add_sys_msg 一路走来似乎没什么可疑的，再向前走走看看吧
				set_sys_msg_mode 3 0 0
				msg_info	
				
			endif
			var Tim 0
			set_task_var 2041 4 8 @Tim 1
		endif
	endif
endproc 