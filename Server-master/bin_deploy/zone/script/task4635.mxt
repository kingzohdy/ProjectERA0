
proc 5
	call 200
	
	COMMIT_TASK 4635 1
endproc

proc 4
	call 200
endproc


proc 200
	GET_TASK_VAR 4635 0 32 124 16
	if @124 = 0 
		if @16 = 1
			GET_TASK_VAR 4635 4 32 125 18
			if @125 = 0
				DEL_ID_MON @18 0
			endif
		elseif @16 = 2
			GET_TASK_VAR 4635 4 32 125 18
			GET_TASK_VAR 4635 8 32 126 19
			if @125 = 0 and @126 = 0
				DEL_ID_MON @18 0
				DEL_ID_MON @19 0
			endif
		elseif @16 = 3
			GET_TASK_VAR 4635 4 32 125 18
			GET_TASK_VAR 4635 8 32 126 19
			GET_TASK_VAR 4635 12 32 127 20
			if @125 = 0 and @126 = 0 and @127 = 0
				DEL_ID_MON @18 0
				DEL_ID_MON @19 0
				DEL_ID_MON @20 0
			endif
		elseif @16 = 4
			GET_TASK_VAR 4635 4 32 125 18
			GET_TASK_VAR 4635 8 32 126 19
			GET_TASK_VAR 4635 12 32 127 20
			GET_TASK_VAR 4635 16 32 128 21
			if @125 = 0 and @126 = 0 and @127 = 0 and @128 = 0
				DEL_ID_MON @18 0
				DEL_ID_MON @19 0
				DEL_ID_MON @20 0
				DEL_ID_MON @21 0
			endif
		else
			SET_TASK_VAR 4635 0 32 0 125
		endif
	endif
endproc

