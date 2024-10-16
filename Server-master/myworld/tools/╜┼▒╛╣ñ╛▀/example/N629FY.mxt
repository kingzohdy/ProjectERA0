;
;script filename: test
;
PROC    1
    ADD_GOODS 3002 1 3

    DIALOG 1
    IF 3 = 0 
        ADD_TALK "OK" 
    ELSE
        ADD_TALK "ERROR"
    ENDIF

    TALK
ENDPROC
