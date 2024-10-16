; Just for test task 


;cancel task , this is a system 
PROC 0

    CANCEL_TASK 1
ENDPROC

;recved task
PROC 1
    GET_TASK_STATUS 10 2

    DIALOG 3

    IF 2 = 0 
        ACCEPT_TASK 10 3
        ADD_TALK 给你个任务吧，去打到一件初行者套装就可以完成了！或者去买一件也行。
        TALK
    ENDIF

    IF 2 = 1
        ACCEPT_TASK 10 3
        ADD_TALK 再做一次？好吧、、、不要太贪心哦
        TALK
    ENDIF
    
    IF 2 = 2 
       GETNUM_GOODS 2801 11 
       IF 11 > 0
           ADD_TALK 恭喜哦，任务完成了耶！给你加点经验值吧。
           DEL_GOODS 2801 1 22
           ADD_EXP 100

           FINISH_TASK 10
       ELSE
           ADD_TALK 你怎么那么笨，还没打到初行者套装呀？
           GET_TASK_VAR 10 0 4
           
           IF 4 = 1 
              SET_TIMER 10 120
           ELSE
              SET_TASK_VAR 10 0 1
              ADD_TASK_HINT 10 1
           ENDIF
       ENDIF

       TALK
    ENDIF


ENDPROC

