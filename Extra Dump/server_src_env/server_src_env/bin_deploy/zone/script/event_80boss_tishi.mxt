
;休斯佩恩提示


proc 1

		; strvar num
		; GET_MAP_MON_NUM 3200 200030 1 num 0
		; if num = 0
			; add_sys_msg_start
			; add_sys_msg 10分钟后修斯佩恩将从#L牢城审讯室@@(3210,13500,13200)#n中苏醒！请各位勇士做好讨伐准备！
			; set_sys_msg_mode 6 0 0
			; svr_msg_info	
		; endif

endproc 

proc 3

		; strvar num
		; GET_MAP_MON_NUM 3200 200030 1 num 0
		; if num = 0
			; GET_NOW_TIME 0 0 0 20 30 40 50
			; if 20 = 6 and 30 = 14 and 40 = 54 and 50 > 49 
				; add_sys_msg_start
				; add_sys_msg 5分钟后修斯佩恩将从#L牢城审讯室@@(3210,13500,13200)#n中苏醒！请各位勇士做好讨伐准备！
				; set_sys_msg_mode 6 0 0
				; svr_msg_info	
			; endif
		; endif
		
endproc
