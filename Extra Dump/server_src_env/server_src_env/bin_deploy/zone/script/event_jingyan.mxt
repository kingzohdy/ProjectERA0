;����

proc 1
        ADD_SYS_MSG_START
        ADD_SYS_MSG ����Ϣ��3��24��-3��30���ڼ䣬ÿ�콫����˫����������������Ͻ��ж�������
        SET_SYS_MSG_MODE 6 0 0
        SVR_MSG_INFO
        
        SET_SVR_STATUS 1052 2 604800 1
		
		strvar time
		get_time_sec time
		
		SET_EVENT_VAR 0 32 @time 0
endproc

PROC 2
        ADD_SYS_MSG_START
        ADD_SYS_MSG ���ֵ�ʱ�����ǹ����ر�죬����7���˫���������������ݸ�һ���䣬ף�����Ϸ��죡
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
