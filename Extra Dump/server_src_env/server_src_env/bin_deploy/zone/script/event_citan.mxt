

proc 1
	strvar Tim1 Tim2 Mid3
	var Tim1 0
	set_event_var 0 32 @Tim1 1
	var Tim2 0
	set_event_var 4 32 @Tim2 1


	CLAN_SPY_OPEN 10 1 6
	if 10 = 0 
		ADD_SYS_MSG_START
		SET_SYS_MSG_MODE 4 0 0
		ADD_SYS_MSG 【公会情报攻防战】一触即发，公会城市的情报官处可以接取任务！
		SVR_MSG_INFO
	
	endif
endproc

proc 2
;结束
	ADD_SYS_MSG_START
	SET_SYS_MSG_MODE 4 0 0
	ADD_SYS_MSG 【公会情报攻防战】暂告一段落
	SVR_MSG_INFO
	CLAN_SPY_CLOSE 1
	
endproc 

proc 3

	strvar ret
	is_clan_spy_open ret
	if ret = 0
		CLAN_SPY_OPEN 1 3 6
	endif
;	get_event_var 4 32 1 Tim2
;	if Tim2 = 0
;		get_event_var 0 32 1 Tim1
;		add Tim1 1
;		set_event_var 0 32 @Tim1 1
;		if Tim1 = 1
;			ADD_SYS_MSG_START
;			SET_SYS_MSG_MODE 4 0 0
;			ADD_SYS_MSG 公会情报战活动已经开启,公会官员可以在公会城市情报官处开启活动任务！
;			SVR_MSG_INFO
;		endif
;		if Tim1 = 2
;			ADD_SYS_MSG_START
;			SET_SYS_MSG_MODE 4 0 0
;			ADD_SYS_MSG 公会情报战活动已经开启,公会官员可以在公会城市情报官处开启活动任务！
;			SVR_MSG_INFO
;			var Tim2 1
;			set_event_var 4 32 @Tim2 1
;		endif
;		if Tim1 = 60
;			ADD_SYS_MSG_START
;			SET_SYS_MSG_MODE 4 0 0
;			ADD_SYS_MSG 公会情报战活动已经开启,公会官员可以在公会城市情报官处开启活动任务！
;			SVR_MSG_INFO
;		endif
;		
;		if Tim1 = 300
;			ADD_SYS_MSG_START
;			SET_SYS_MSG_MODE 4 0 0
;			ADD_SYS_MSG 公会情报战活动已经开启,公会官员可以在公会城市情报官处开启活动任务！
;			SVR_MSG_INFO
;			var Tim2 1
;			set_event_var 4 32 @Tim2 1
;		endif
;	endif

	
endproc 