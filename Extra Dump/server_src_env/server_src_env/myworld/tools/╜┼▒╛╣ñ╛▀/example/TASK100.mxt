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
        ADD_TALK ���������ɣ�ȥ��һ����������װ�Ϳ�������ˣ�����ȥ��һ��Ҳ�С�
        TALK
    ENDIF

    IF 2 = 1
        ACCEPT_TASK 10 3
        ADD_TALK ����һ�Σ��ðɡ�������Ҫ̫̰��Ŷ
        TALK
    ENDIF
    
    IF 2 = 2 
       GETNUM_GOODS 2801 11 
       IF 11 > 0
           ADD_TALK ��ϲŶ�����������Ү������ӵ㾭��ֵ�ɡ�
           DEL_GOODS 2801 1 22
           ADD_EXP 100

           FINISH_TASK 10
       ELSE
           ADD_TALK ����ô��ô������û�򵽳�������װѽ��
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

