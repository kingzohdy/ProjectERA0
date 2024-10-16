;牧牛预赛,预赛牧牛任务
;接受事件
proc 1 
;	NEW_CONVOY_MON ::cow:: 20600 31800 200 0 0 0 1800 30022 1 2
	var 2 0
	SET_TASK_VAR 30022 0 32 @2 1
endproc
;放弃事件
proc 5
	COMMIT_TASK 30022 1
	TASK_STAT 30022 1 11
	;不是未完成,无视
	if 11 != 2
		return
	endif
	;卸磨杀牛
	GET_TASK_VAR 30022 0 32 1 10
	if 10 != 0
		DEL_ID_MON @10 1
	endif	
endproc
