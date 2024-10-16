;经验活动

proc 1
        ADD_SYS_MSG_START
        ADD_SYS_MSG 好消息！3月24日-3月30日期间，每天将享受双倍经验大酬宾！还不赶紧行动起来！
        SET_SYS_MSG_MODE 6 0 0
        SVR_MSG_INFO
        
        SET_SVR_STATUS 1052 2 604800 1
		
		strvar time
		get_time_sec time
		
		SET_EVENT_VAR 0 32 @time 0
endproc

PROC 2
        ADD_SYS_MSG_START
        ADD_SYS_MSG 欢乐的时间总是过得特别快，持续7天的双倍经验大酬宾活动现已暂告一段落，祝大家游戏愉快！
        SET_SYS_MSG_MODE 6 0 0
        SVR_MSG_INFO
        
ENDPROC

proc 4
	strvar time
	GET_EVENT_VAR 0 32 0 time
	
	strvar dangqian
	get_time_sec dangqian
	
	add time 604800
	sub time @dangqian
	
	strvar song
	GET_EVENT_STAT song 305
	
	if song = 1
		SET_SVR_STATUS 1052 2 @time 1
	endif
endproc
