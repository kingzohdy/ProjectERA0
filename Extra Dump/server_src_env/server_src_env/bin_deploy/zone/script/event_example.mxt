;��ȯ����

proc 1
        ADD_SYS_MSG_START
        ADD_SYS_MSG ����Ϣ��20:00-22:00�ڼ䣬ɱ�ֽ����˫�����飬����ÿ������ѻ�ý�ȯ�������Ͻ��ж�������
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
        ADD_SYS_MSG ���ֵ�ʱ�����ǹ����ر�죬ת�۾�22:00�ˣ������ɱ�ֻ��˫�����飬��ÿ����������ͽ�ȯ����ݸ�һ����
        SET_SYS_MSG_MODE 6 0 0
        SVR_MSG_INFO
        
        GET_EVENT_VAR 0 32 1 2

	strvar sec
	get_time_sec sec
	sub sec @2

	;60��	
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
