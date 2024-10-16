;血色追随者依莫顿


proc 2

	TASK_STAT 1519 1 20
	if 20 = 2 

			GET_MON_INFO 10 11 12 13 14 15 16 17 
			CALL_MON 1 293074 0 60 @16 @17 1	

			add_sys_msg_start
			add_sys_msg 发现了一个宝箱，快打开来看看。
			set_sys_msg_mode 3 0 0
			msg_info

	endif
endproc

