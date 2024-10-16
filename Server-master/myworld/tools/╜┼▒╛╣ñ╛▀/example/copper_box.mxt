;
; script file : box.mxt 
; 

; proc number  :
;   10 , means this proc will be executed while initializing this box monster...
;   11 , means this proc will be executed while this box monster be hited 
;   12 , means this proc will be executed when this box monster died 
;   13 , means this proc will be executed when this box monster revived 

PROC  10
    SET_MONS_REVIVE_TIME 10

ENDPROC

PROC 11
    CHECK_BOX_KEY 3303 你没有铜钥匙，打不开宝箱！
ENDPROC

PROC 12
    SET_MONS_REVIVE_TIME 300
ENDPROC

PROC 13
    RANDOM_POS  40  41 42 
    IF 42 = 0
        SET_MONS_REVIVE_POS  40 41 
    ENDIF

ENDPROC
