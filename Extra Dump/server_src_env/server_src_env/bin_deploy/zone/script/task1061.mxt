;;;;打木桩

proc 1
	strvar Tim
	var Tim 1
	set_task_var 1061 0 8 @Tim 1

endproc 

proc 6
	strvar Map
	GET_ID_PLAYER_INFO 0 5 Map
	if Map = 2000 or Map = 2001 or Map = 2002 or Map = 2003 or Map = 2004 or Map = 2005 or Map = 2006 or Map = 2007

		strvar Tim Rac
		get_task_var 1061 0 8 1 Tim
		add Tim 1
		set_task_var 1061 0 8 @Tim 1
		if Tim = 15
			
			GET_ID_PLAYER_INFO 0 7 Rac
			if Rac = 1
				add_sys_msg_start
				add_sys_msg 任务【实力的证明】：对木桩使用新学会的技能：撕裂
				set_sys_msg_mode 3 0 0
				msg_info
			endif
			if Rac = 2
				add_sys_msg_start
				add_sys_msg 任务【实力的证明】：对木桩使用新学会的技能：冰箭术 
				set_sys_msg_mode 3 0 0
				msg_info
			endif
			if Rac = 3
				add_sys_msg_start
				add_sys_msg 任务【实力的证明】：对木桩使用新学会的技能：震荡射击   
				set_sys_msg_mode 3 0 0
				msg_info
			endif
			if Rac = 4
				add_sys_msg_start
				add_sys_msg 任务【实力的证明】：对木桩使用新学会的技能：雷击术    
				set_sys_msg_mode 3 0 0
				msg_info
			endif
			
			var Tim 0
			set_task_var 1061 0 8 @Tim 1
		
		endif
	endif
endproc 