;;;;;;;;;;;村民


proc 2
;	strvar Pmid Ret val Mmid
;	get_mon_var 0 32 1 Pmid   
;	GET_MON_INFO 1 1 Mmid 1 1 1 1 1 
;	
;	SET_PLAYER_CURR @Pmid Ret
;	if Ret = 0
;		get_task_var 1544 16 8 1 val
;		sub val 1
;		set_task_var 1544 16 8 @val 1
;		strvar TaRet
;		TASK_STAT 1544 1 TaRet
;		if TaRet = 2
;			MON_SPEECH @Mmid 1 0 1 我还是回到人群中去吧，那样更有安全感……
;		endif	
;	endif
	
endproc 
