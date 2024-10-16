;1852

proc 3
	strvar ID
	get_player_id 1 ID
	TARGET_ADD_STATUS 1 @ID 1249 1 0 1		
endproc 
proc 4
	strvar ID
	get_player_id 1 ID
	TARGET_ADD_STATUS 1 @ID 1249 1 0 1		
endproc 
proc 5
	strvar ID
	get_player_id 1 ID
	TARGET_ADD_STATUS 1 @ID 1249 1 0 1		
endproc 


proc 7

	get_task_var 1852 0 8 1 20
	if 20 = 1
		strvar Ret
		HAS_STATUS 1249 Ret
		
		if Ret = -1
			TASK_RUN 1852 1
			add_sys_msg_start
			add_sys_msg 必须要在古树的祝福消失之前提交任务，赶快去获得祝福吧！
			set_sys_msg_mode 1 0 0
			msg_info
			
			var 20 0
			set_task_var 1852 0 8 @20 1
		endif
	endif	

endproc 





