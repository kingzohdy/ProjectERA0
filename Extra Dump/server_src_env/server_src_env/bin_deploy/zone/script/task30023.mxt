;牧牛决赛,决赛牧牛任务
;接受事件
proc 1 
;	NEW_CONVOY_MON ::cow:: 17936 33838 200 0 0 0 1800 30023 1 2
	var 2 0
	SET_TASK_VAR 30023 0 32 @2 1
endproc
;放弃事件
proc 5
	COMMIT_TASK 30023 1
	TASK_STAT 30023 1 11
	;不是未完成,无视
	if 11 != 2
		return
	endif
	;卸磨杀牛
	GET_TASK_VAR 30023 0 32 1 10
	if 10 != 0
		DEL_ID_MON @10 1
	endif	
endproc
