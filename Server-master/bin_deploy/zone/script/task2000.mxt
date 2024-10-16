;牧牛任务,牧牛任务
;接受事件
proc 1 
	NEW_CONVOY_MON 213018 2639 5225 500 0 0 0 360 2000 1 2
	SET_TASK_VAR 2000 0 32 @2 1
endproc
;放弃事件
proc 5
	TASK_STAT 2000 1 11
	;不是未完成,无视
	if 11 != 2
		return
	endif
	;卸磨杀牛
	GET_TASK_VAR 2000 0 32 1 10
	DEL_ID_MON @10 1
endproc
