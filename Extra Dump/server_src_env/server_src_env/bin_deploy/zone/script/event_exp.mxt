; ���⺣̲���鷭��

proc 1
        ADD_SYS_MSG_START
        ADD_SYS_MSG ����Ϣ��18:00-24:00�ڼ䣬��ԡ���⺣̲��õľ��齫���һ����
        SET_SYS_MSG_MODE 1 0 0
        SVR_MSG_INFO

        SET_SVR_STATUS 3085 2 21600 1
endproc

PROC 2
        ADD_SYS_MSG_START
        ADD_SYS_MSG ���ֵ�ʱ�����ǹ����ر�죬ת�۾�24:00�ˣ�������ԡ���⺣̲��÷�������Ļ���ݸ�һ����
        SET_SYS_MSG_MODE 1 0 0
        SVR_MSG_INFO
ENDPROC

proc 4
	strvar haitan
	GET_EVENT_STAT haitan 303
	
	if haitan = 1
		SET_SVR_STATUS 3085 2 21600 1
	endif
endproc
