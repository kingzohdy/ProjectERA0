;����Ǳ�ʯ�ӹ�ʦ
;ʹ����ʱ����824
;
PROC 1
    GET_TASK_VAR  001 0 1003
    IF 1003 = 20
    	DIALOG 3
    	ADD_TALK $_npc$��
    	ADD_TALK ����001��ɣ�������100��ң�
    	TALK 
    	SET_TASK_VAR 001 0 30
    	FINISH_TASK 001
    	ADD_TASK_HINT 001 03
    	ADD_MONEY 100
    	RETURN
    ENDIF 
    GET_TASK_VAR  002 0 1006
    IF 1006 = 20
    	DIALOG 3
    	ADD_TALK $_npc$��
    	ADD_TALK ����002��ɣ�������100��ң�
    	TALK 
    	SET_TASK_VAR 002 0 30
    	FINISH_TASK 002
    	ADD_TASK_HINT 002 03
    	ADD_MONEY 100
    	RETURN 
    ENDIF 
    GET_TASK_VAR  003 0 1009
    IF 1009 = 20
    	DIALOG 3
    	ADD_TALK $_npc$��
    	ADD_TALK ����003��ɣ�������100��ң�
    	TALK 
    	SET_TASK_VAR 003 0 30
    	FINISH_TASK 003
    	ADD_TASK_HINT 003 03
    	ADD_MONEY 100
    	RETURN 
    ENDIF 
    GET_TASK_VAR  004 0 1012
    IF 1012 = 20
    	DIALOG 3
    	ADD_TALK $_npc$��
    	ADD_TALK ����004��ɣ�������100��ң�
    	TALK 
    	SET_TASK_VAR 004 0 30
    	FINISH_TASK 004
    	ADD_TASK_HINT 004 03
    	ADD_MONEY 100
    	RETURN 
    ENDIF 
    GET_TASK_VAR  005 0 1015
    IF 1015 = 20
    	DIALOG 3
    	ADD_TALK $_npc$��
    	ADD_TALK ����005��ɣ�������100��ң�
    	TALK 
    	SET_TASK_VAR 005 0 30
    	FINISH_TASK 005
    	ADD_TASK_HINT 005 03
    	ADD_MONEY 100
    	RETURN 
    ENDIF 
    GET_TASK_VAR  006 0 1018
    IF 1018 = 20
    	DIALOG 3
    	ADD_TALK $_npc$��
    	ADD_TALK ����006��ɣ�������100��ң�
    	TALK 
    	SET_TASK_VAR 006 0 30
    	FINISH_TASK 006
    	ADD_TASK_HINT 006 03
    	ADD_MONEY 100
    	RETURN 
    ENDIF 
    GET_TASK_VAR  007 0 1021
    IF 1021 = 20
    	DIALOG 3
    	ADD_TALK $_npc$��
    	ADD_TALK ����007��ɣ�������100��ң�
    	TALK 
    	SET_TASK_VAR 007 0 30
    	FINISH_TASK 007
    	ADD_TASK_HINT 007 03
    	ADD_MONEY 100
    	RETURN 
    ENDIF 
    GET_TASK_VAR  008 0 999
    IF 999 = 20
    	DIALOG 3
    	ADD_TALK $_npc$��
    	ADD_TALK ����008��ɣ�������100��ң�
    	TALK 
    	SET_TASK_VAR 008 0 30
    	FINISH_TASK 008
    	ADD_TASK_HINT 008 03
    	ADD_MONEY 100
    	RETURN 
    ENDIF    
    GET_TASK_VAR  009 0 996
    IF 996 = 20
    	DIALOG 3
    	ADD_TALK $_npc$��
    	ADD_TALK ����009��ɣ�������100��ң�
    	TALK 
    	SET_TASK_VAR 009 0 30
    	FINISH_TASK 009
    	ADD_TASK_HINT 009 03
    	ADD_MONEY 100
    	RETURN 
    ENDIF  
    GET_TASK_VAR  010 0 993
    IF 993 = 20
    	DIALOG 3
    	ADD_TALK $_npc$��
    	ADD_TALK ����010��ɣ�������100��ң�
    	TALK 
    	SET_TASK_VAR 010 0 30
    	FINISH_TASK 010
    	ADD_TASK_HINT 010 03
    	ADD_MONEY 100
    	RETURN 
    ENDIF
    IF 993 = 50
    	DIALOG 3
    	ADD_TALK $_npc$��
    	ADD_TALK ����010��ɣ�������100��ң�
    	TALK 
    	SET_TASK_VAR 010 0 30
    	FINISH_TASK 010
    	ADD_TASK_HINT 010 03
    	ADD_MONEY 100
    	RETURN 
    ENDIF
    GET_TASK_VAR  011 0 990
    IF 990 = 20
    	DIALOG 3
    	ADD_TALK $_npc$��
    	ADD_TALK ����011��ɣ�������100��ң�
    	TALK 
    	SET_TASK_VAR 011 0 30
    	FINISH_TASK 011
    	ADD_TASK_HINT 011 03
    	ADD_MONEY 100
    	RETURN 
    ENDIF
    
    DIALOG 1
    ADD_TALK $_npc$��
    ADD_TALK �η熪�����ꆪ����ʼ�����񆪣�
    OPTION 10 ��һ������
    OPTION 20 �ڶ�������
    OPTION 30 ����������
    TALK
ENDPROC
;
;
PROC 10
    DIALOG 1
    ADD_TALK $_npc$��
    ADD_TALK ����Ҫ�Եģ�����Ҫ˯�ģ�������Ҫ��ģ�
    OPTION 11 ����001
    OPTION 12 ����002
    OPTION 13 ����003
    OPTION 14 ����004
    OPTION 15 ����005
    TALK
ENDPROC
;
;
PROC 20
    DIALOG 1
    ADD_TALK $_npc$��
    ADD_TALK �����������Ǹ֣����񲻲�����ţ�
    OPTION 21 ����006
    OPTION 22 ����007
    OPTION 23 ����008
    OPTION 24 ����009
    OPTION 25 ����010
    TALK
ENDPROC
;
;
PROC 30
    DIALOG 1
    ADD_TALK $_npc$��
    ADD_TALK ��Ҫ����������ø���̬Щ�ɣ�
    OPTION 31 ����011
    ;OPTION 32 ����012
    ;OPTION 33 ����013
    ;OPTION 34 ����014
    ;OPTION 35 ����025
    TALK
ENDPROC 
;
;
PROC 11
    EXEC TASK001 1    
ENDPROC   
;
;
PROC 12
    EXEC TASK002 1    
ENDPROC  
;
;
PROC 13
    EXEC TASK003 1    
ENDPROC  
;
;
PROC 14
    EXEC TASK004 1    
ENDPROC  
;
;
PROC 15
    EXEC TASK005 1    
ENDPROC  
;
;
PROC 21
    EXEC TASK006 1    
ENDPROC  
;
;
PROC 22
    EXEC TASK007 1    
ENDPROC  
;
;
PROC 23
    EXEC TASK008 1    
ENDPROC  
;
;
PROC 24
    EXEC TASK009 1    
ENDPROC  
;
;
PROC 25
    EXEC TASK010 1    
ENDPROC  
;
;
PROC 31
    EXEC TASK011 1    
ENDPROC 
;

