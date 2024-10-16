;图腾招怪任务
proc 1

		VAR 10 0
		SET_TASK_VAR 1067 0 8 @10 1
		VAR 11 0
		SET_TASK_VAR 1067 1 8 @11 1

endproc 
proc 3
	get_task_var 1067 2 32 1 12
	DEL_ID_MON @12 1

endproc

proc 5

	get_task_var 1067 2 32 1 12
	DEL_ID_MON @12 1
	
endproc
proc 6
	
		GET_TASK_VAR 1067 0 8 1 10
		if 10 = 1 
			
			GET_TASK_VAR 1067 1 8 1 11
			add 11 1
			SET_TASK_VAR 1067 1 8 @11 1
			if 11 = 20
				task_fini 1067 1
				add_sys_msg_start
				add_sys_msg 你成功的保护了图腾的安全，任务完成！
				set_sys_msg_mode 3 0 0
				msg_info
			
			endif
		endif

endproc
