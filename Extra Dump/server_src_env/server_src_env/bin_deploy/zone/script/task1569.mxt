
proc 1
	var 10 0
	set_task_var 1569 0 8 @10 1

	strvar tim
	set_task_var 1569 1 32 @tim 1
endproc 

proc 2

	strvar Mid1 Mid2 Mid3
	
	get_task_var 1569 0 32 1 Mid1 
	get_task_var 1569 4 32 1 Mid2 
	get_task_var 1569 8 32 1 Mid3 
	
	DEL_ID_MON @Mid1 10
	DEL_ID_MON @Mid2 11
	DEL_ID_MON @Mid3 12
	
endproc

proc 5
	strvar Mid1 Mid2 Mid3
	
	get_task_var 1569 0 32 1 Mid1 
	get_task_var 1569 4 32 1 Mid2 
	get_task_var 1569 8 32 1 Mid3 
	
	DEL_ID_MON @Mid1 10
	DEL_ID_MON @Mid2 11
	DEL_ID_MON @Mid3 12

endproc 
proc 6
	get_task_var 1569 0 8 1 10
	if 10 = 1
		get_task_var 1569 1 32 1 tim
		add tim 1
		set_task_var 1569 1 32 @tim 1
		if tim = 240 
			var 10 0
			set_task_var 1569 0 8 @10 1
			var tim 0
			set_task_var 1569 1 32 @tim 1
		endif
	endif
endproc 