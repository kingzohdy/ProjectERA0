

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
		ADD_SYS_MSG �������鱨����ս��һ��������������е��鱨�ٴ����Խ�ȡ����
		SVR_MSG_INFO
	
	endif
endproc

proc 2
;����
	ADD_SYS_MSG_START
	SET_SYS_MSG_MODE 4 0 0
	ADD_SYS_MSG �������鱨����ս���ݸ�һ����
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
;			ADD_SYS_MSG �����鱨ս��Ѿ�����,�����Ա�����ڹ�������鱨�ٴ����������
;			SVR_MSG_INFO
;		endif
;		if Tim1 = 2
;			ADD_SYS_MSG_START
;			SET_SYS_MSG_MODE 4 0 0
;			ADD_SYS_MSG �����鱨ս��Ѿ�����,�����Ա�����ڹ�������鱨�ٴ����������
;			SVR_MSG_INFO
;			var Tim2 1
;			set_event_var 4 32 @Tim2 1
;		endif
;		if Tim1 = 60
;			ADD_SYS_MSG_START
;			SET_SYS_MSG_MODE 4 0 0
;			ADD_SYS_MSG �����鱨ս��Ѿ�����,�����Ա�����ڹ�������鱨�ٴ����������
;			SVR_MSG_INFO
;		endif
;		
;		if Tim1 = 300
;			ADD_SYS_MSG_START
;			SET_SYS_MSG_MODE 4 0 0
;			ADD_SYS_MSG �����鱨ս��Ѿ�����,�����Ա�����ڹ�������鱨�ٴ����������
;			SVR_MSG_INFO
;			var Tim2 1
;			set_event_var 4 32 @Tim2 1
;		endif
;	endif

	
endproc 