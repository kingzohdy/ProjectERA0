;烟火信号
;任务——1022
;作用——使用后删除，并完成任务
proc 1
	get_player_mapinfo 0 1 2 3 4
	get_point_dist @3 @4 14015 34238 10
	if 2 = 2000 OR 2 = 2001 OR 2 = 2002 OR 2 = 2003 or 2 = 2004 or 2 = 2005 or 2 = 2006 or 2 = 2007 
		if 10 <= 500  
			effect 0 0 playEffct(14015,34238,"effect\\entity\\9513_16.ent")
			dec_item_num_inpak 5300134 1 0
			autofly 104 1
;			task_fini 1074 0
			
			GET_TASK_VAR 1074 0 8 1 10
			var 10 1
			SET_TASK_VAR 1074 0 8 @10 1

			
		else
			add_sys_msg_start
			add_sys_msg 不是在这个区域！
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	ELSE
			add_sys_msg_start
			add_sys_msg 目标不再指定地图中！
			set_sys_msg_mode 3 0 0
			msg_info
	ENDIF
endproc

