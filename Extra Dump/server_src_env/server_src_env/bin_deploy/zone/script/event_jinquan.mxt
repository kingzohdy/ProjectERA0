
PROC 1
	call 100

	strvar sec
	get_time_sec sec

	SET_EVENT_VAR 0 32 @sec 1
	; ADD_SYS_MSG_START
	; SET_SYS_MSG_MODE 4 0 0
	; ADD_SYS_MSG �����ٴ�����������½�����߻񽫻�÷������ҽ���
	; SVR_MSG_INFO
ENDPROC

PROC 2
	GET_EVENT_VAR 0 32 1 2

	strvar sec
	get_time_sec sec
	sub sec @2

	;60��	
	if sec >= 60
		call 100
	endif

	; ADD_SYS_MSG_START
	; SET_SYS_MSG_MODE 4 0 0
	; ADD_SYS_MSG ����������Ļ����л���Ĳ���
	; SVR_MSG_INFO
ENDPROC

proc 3
	GET_EVENT_VAR 0 32 1 2
	;add 2 1
	strvar sec
	get_time_sec sec	
	var 3 @sec
	sub sec @2

	;600��	
	if sec < 60
		return
	endif
	SET_EVENT_VAR 0 32 @3 1
	
	call 100
endproc

proc 100
	; ADD_SYS_MSG_START
	; SET_SYS_MSG_MODE 4 0 0
	; ADD_SYS_MSG ���������ʹ���19��-22���ڼ����߻�����ҽ���
	; SVR_MSG_INFO

	; strvar X Y
	; GET_PLAYER_MAPINFO 0 0 0 X Y

;	ADD_ONLINE_ATTR 1 2 1 "effect\\entity\\3110003.ent"
	ADD_ONLINE_ATTR 1 3 1 0

endproc