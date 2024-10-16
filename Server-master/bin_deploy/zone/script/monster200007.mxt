
proc 2
;	STRVAR x y ret
;	task_stat 1717 1 ret
;	if ret = 2
;		GET_MON_INFO 1 0 0 0 0 0 X Y
;		NEW_LIFE_MON 235010 1 @x @y 0 0 0 0 15 1 0
;	endif
	
	strvar Ret x y
	MON_OWNER_TASK_CHECK 1717 Ret
		if Ret = 0
			GET_MON_INFO 1 0 0 0 0 0 X Y
			NEW_LIFE_MON 235010 1 @x @y 0 0 0 0 15 1 0
		endif
endproc

