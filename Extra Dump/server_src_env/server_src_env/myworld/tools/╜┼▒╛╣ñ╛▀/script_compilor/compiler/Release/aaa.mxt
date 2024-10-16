;
;script filename: aaa
;
PROC    0
    RAND 10 10
    IF (1 > 0 OR 2 > 0) AND NOT (3 > 0 or 4 > 0 or not 5 > 0 and 6 > 0 and (7 > 0 or 8 > 0) and 9 > 0)
	    CLEAR_TALK 'A'
    ELSEIF (1 > 0 OR 2 < 0) AND NOT (3 < 0 or 4 < 0 or not 5 < 0 and 6 < 0 and (7 < 0 or 8 < 0) and 9 < 0)
	    ADD_TALK 'V' script_aaa_proc_0_else
    ELSEIF (1 < 0 OR 2 > 0) AND NOT (3 < 0 or 4 > 0 or not 5 < 0 and 6 > 0 and (7 < 0 or 8 > 0) and 9 < 0)
	    CLEAR_ANI
    ELSE
	    CLEAR_MAGIC 6
    ENDIF
    ADD 10 1	
ENDPROC