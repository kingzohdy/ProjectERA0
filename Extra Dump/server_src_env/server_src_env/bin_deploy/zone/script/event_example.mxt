;金券经验活动

proc 1
        ADD_SYS_MSG_START
        ADD_SYS_MSG 好消息！20:00-22:00期间，杀怪将获得双倍经验，还能每分钟免费获得金券！还不赶紧行动起来！
        SET_SYS_MSG_MODE 6 0 0
        SVR_MSG_INFO

        SET_SVR_STATUS 1052 2 7200 1
        
        call 100

	strvar sec
	get_time_sec sec

	SET_EVENT_VAR 0 32 @sec 1
endproc

PROC 2
        ADD_SYS_MSG_START
        ADD_SYS_MSG 欢乐的时间总是过得特别快，转眼就22:00了，今天的杀怪获得双倍经验，和每分钟免费增送金券活动已暂告一段落
        SET_SYS_MSG_MODE 6 0 0
        SVR_MSG_INFO
        
        GET_EVENT_VAR 0 32 1 2

	strvar sec
	get_time_sec sec
	sub sec @2

	;60秒	
	if sec >= 60
		call 100
	endif
ENDPROC

proc 3
	GET_EVENT_VAR 0 32 1 2
	;add 2 1
	strvar sec
	get_time_sec sec	
	var 3 @sec
	sub sec @2

	if sec < 60
		return
	endif
	SET_EVENT_VAR 0 32 @3 1
	
	call 100
ENDPROC

proc 100
	ADD_ONLINE_ATTR 1 3 1 0
endproc
