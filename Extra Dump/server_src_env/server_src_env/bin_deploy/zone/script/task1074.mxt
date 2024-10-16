proc 1
		VAR 10 0
		SET_TASK_VAR 1074 0 8 @10 1
		VAR 11 0
		SET_TASK_VAR 1074 1 8 @11 1
		
endproc
proc 6
		GET_TASK_VAR 1074 0 8 1 10
		if 10 = 1 
			
			GET_TASK_VAR 1074 1 8 1 11
			add 11 1
			SET_TASK_VAR 1074 1 8 @11 1
			if 11 = 3
				task_fini 1074 1
				add_sys_msg_start
				add_sys_msg 烟花落尽，一阵熟悉的龙吟响彻天际……
				set_sys_msg_mode 3 0 0
				msg_info
			
			endif
		endif

endproc
