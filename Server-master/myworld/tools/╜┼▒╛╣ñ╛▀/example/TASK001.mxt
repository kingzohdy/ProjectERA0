; Just for test task 


;cancel task , this is a system 
PROC 0

    CANCEL_TASK 1
ENDPROC

;recved task
PROC 1
    GET_TASK_STATUS 1 2

    DIALOG 1

    IF 2 = 0 
        ADD_TALK Give u a task... 
        ACCEPT_TASK 1 3
    ENDIF
    
    IF 2 = 2 
       GETNUM_GOODS 2801 11 
       IF 11 > 0
           ADD_TALK U had finished this task !
       ELSE
           ADD_TALK U hadn't finished this task yet ?
       ENDIF
    ENDIF

    IF 2 = 2
       IF 11 > 0  
           NEXT 21
       ELSE
           ADD_TASK_HINT 1 1
           ADD_TASK_HINT 1 2
           ADD_TASK_HINT 1 3
       ENDIF
    ENDIF
    
    TALK

ENDPROC

;finish task
PROC 21
    ADD_GOODS 2802 3 4

    FINISH_TASK 1
ENDPROC
