;����2����Ա

PROC 1 
	TASK_FINI 1102 1
	DIALOG 1

	
	ADD_TALK ���ҵ�����֮��,�����м�ð����ʿ�����ӹ���յ���ѵ����
	
    TASK_ADD_OPTION 1013 1 101 
    TASK_ADD_OPTION 1012 1 102 
	TASK_ADD_OPTION 1014 1 103 
    TASK_ADD_OPTION 4000 1 104 
	
	task_stat 1013 33 63
	if 63 = 3
		OPTION 105 #���븱��2
	endif
	
	if 63 = 2
		OPTION 105 #���븱��2
	endif
	
	OPTION 106 #���л�����
	
	option 107 #���ø���
    TALK               
ENDPROC

PROC 101
	TASK_DO_OPTION 1013 1
ENDPROC  

PROC 102
    TASK_DO_OPTION 1012 1
ENDPROC 

PROC 103
	TASK_DO_OPTION 1014 1
ENDPROC

PROC 104
	TASK_DO_OPTION 4000 1
ENDPROC

PROC 105
	ENTER_PWORLD 501 0 1
	autofly 2 1
ENDPROC  

PROC 106
	AUTOFLY 1 1
	TASK_FINI 1008 1
ENDPROC

proc 107
	reset_pworld 501
	ADD_SYS_MSG_START
	ADD_SYS_MSG ����������
	SET_SYS_MSG_MODE 4 0 0
	MAP_MSG_INFO
endproc
	
proc 2
	get_trig_task 600 800
	if 800 = 1014
		task_fini 1014 64
	endif
	
	if 800 = 1013
		enter_pworld 501 0 1
	endif
endproc
