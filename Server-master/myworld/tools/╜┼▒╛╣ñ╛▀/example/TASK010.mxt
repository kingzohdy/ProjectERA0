;����010
;
PROC 0
    CANCEL_TASK 010
ENDPROC
;
;
PROC 1
    GET_TASK_NUM 1000  
    IF 1000 >= 10
    	DIALOG 3
    	ADD_TALK $_npc$��
    	ADD_TALK �Բ�������10�������ڽ����У��޷�����ȡ�µ�����
	TALK 
    ENDIF 
    GET_TASK_STATUS 010 995
    IF 995 = 0
        DIALOG 1
        ADD_TALK $_npc$��
        ADD_TALK ��������赸ʦ˵���أ����������ʦ˵����
        OPTION 40 �赸ʦ
        OPTION 41 ��ʦ
        TALK       
    ENDIF     
    IF 995 = 1
    	DIALOG 3
    	ADD_TALK $_npc$��
    	ADD_TALK ������ɴ�����
	TALK 
    ENDIF 
    IF 995 = 2
    	DIALOG 3
    	ADD_TALK $_npc$��
    	ADD_TALK �㲻�����������������
    ENDIF 	
ENDPROC    

;
PROC 40
    DIALOG 3
    ADD_TALK $_npc$��
    ADD_TALK ����ȥ���赸ʦ˵���ɣ�
    TALK
    ACCEPT_TASK 010 994
    SET_TASK_VAR 010 0 10
    ADD_TASK_HINT 010 01  
    SET_TASK_FLAG 010 0
ENDPROC 
;
;
PROC 41
    DIALOG 3
    ADD_TALK $_npc$��
    ADD_TALK ����ȥ����ʦ˵���ɣ�������ݵĶ�¥��
    TALK
    ACCEPT_TASK 010 994
    SET_TASK_VAR 010 0 40
    ADD_TASK_HINT 010 04  
    SET_TASK_FLAG 010 1
ENDPROC 

