

proc 3
;提交删除

endproc 

proc 5
;放弃任务

	get_task_var 1623 0 32 1 10
	DEL_ID_MON @10 0
	
endproc

