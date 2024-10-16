
;牛仔很忙、魔影重重提示


proc 1

	

endproc 

proc 3

	GET_NOW_TIME 0 0 0 20 30 40 50
	if 20 = 7 and 30 = 18 and 40 = 49 and 50 > 49
	;周日17:00
		add_sys_msg_start
		add_sys_msg “魔影重重”活动将在10分钟后开启！请各位勇士做好讨伐准备！
		set_sys_msg_mode 6 0 0
		svr_msg_info	
	endif
	if 20 = 7 and 30 = 18 and 40 = 54 and 50 > 49
	;周日17:00
		add_sys_msg_start
		add_sys_msg “魔影重重”活动将在5分钟后开启！请各位勇士做好讨伐准备！
		set_sys_msg_mode 6 0 0
		svr_msg_info	
	endif
	
	if 20 = 1 or 20 = 4
	;周一 周四 8点20
		if 30 = 20 and 40 = 19 and 50 > 49
			add_sys_msg_start
			add_sys_msg 彩虹岛的“浩东”将在10分钟后打开“牛仔很忙-决赛”的大门！请拥有决赛资格的玩家做好准备！
			set_sys_msg_mode 6 0 0
			svr_msg_info
			
		endif
		if 30 = 20 and 40 = 24 and 50 > 49
			add_sys_msg_start
			add_sys_msg 彩虹岛的“浩东”将在5分钟后打开“牛仔很忙-决赛”的大门！请拥有决赛资格的玩家做好准备！
			set_sys_msg_mode 6 0 0
			svr_msg_info
			
		endif
	endif
	
	
;	strvar num
;	GET_MAP_MON_NUM 3200 200030 1 num 0
;	if num = 0
;		GET_NOW_TIME 0 0 0 20 30 40 50
;		if 30 = 18 and 40 = 54 and 50 > 49
;			add_sys_msg_start
;			add_sys_msg 5分钟后修斯佩恩将从#L牢城@@(3200,15900,14500)#n中苏醒！请各位勇士做好讨伐准备！
;			set_sys_msg_mode 6 0 0
;			svr_msg_info	
;		endif
;	endif

endproc

