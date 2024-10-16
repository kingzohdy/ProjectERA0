;任务010
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
    	ADD_TALK $_npc$：
    	ADD_TALK 对不起，你有10个任务在进行中，无法再领取新的任务。
	TALK 
    ENDIF 
    GET_TASK_STATUS 010 995
    IF 995 = 0
        DIALOG 1
        ADD_TALK $_npc$：
        ADD_TALK 你是想和舞蹈师说话呢？还是想和武师说话？
        OPTION 40 舞蹈师
        OPTION 41 武师
        TALK       
    ENDIF     
    IF 995 = 1
    	DIALOG 3
    	ADD_TALK $_npc$：
    	ADD_TALK 你已完成此任务。
	TALK 
    ENDIF 
    IF 995 = 2
    	DIALOG 3
    	ADD_TALK $_npc$：
    	ADD_TALK 你不是正在做这个任务吗？
    ENDIF 	
ENDPROC    

;
PROC 40
    DIALOG 3
    ADD_TALK $_npc$：
    ADD_TALK 那你去和舞蹈师说话吧！
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
    ADD_TALK $_npc$：
    ADD_TALK 那你去和武师说话吧！他在武馆的二楼。
    TALK
    ACCEPT_TASK 010 994
    SET_TASK_VAR 010 0 40
    ADD_TASK_HINT 010 04  
    SET_TASK_FLAG 010 1
ENDPROC 

