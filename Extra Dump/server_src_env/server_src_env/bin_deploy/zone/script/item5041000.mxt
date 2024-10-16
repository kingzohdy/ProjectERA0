;»Ø³Ì¾íÖá
proc 1
;	strvar Map
;	
;	
;	TASK_STAT 3143 1 20
;	TASK_STAT 3148 1 21
;	TASK_STAT 3161 1 22
;	TASK_STAT 3162 1 23
;	TASK_STAT 3158 1 24
;	
;	GET_PLAYER_MAPINFO 1 Map 1 1 1
;	
;	add_sys_msg_start
;	add_sys_msg $@Map$ $@20$
;	set_sys_msg_mode 1 0 0
;	msg_info
;	
;	if Map = 50500 or Map = 50600 or Map = 50700 or Map = 50800
;		if 20 = 2
;			TASK_FAIL 3143 1
;		endif
;		if 21 = 2
;			TASK_FAIL 3148 1
;		endif
;		if 22 = 2 
;			TASK_FAIL 3161 1
;		endif
;		if 23 = 2
;			TASK_FAIL 3162 1
;		endif
;		if 24 = 2
;			TASK_FAIL 3158 1
;		endif
;	endif
endproc 