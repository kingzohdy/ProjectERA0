proc 1
	STRVAR a
	get_task_var 1711 0 32 1 a
	var a 0
	set_task_var 1711 0 32 @a 1
endproc
proc 5
	get_task_var 1711 0 32 1 a
		if 1 = -1
			return
		endif
	SET_MON_CURR @a 1
		if 1 = -1
			return
		else
			DESTROY_MON 1
				if 1 = -1
					return
				endif
		endif
endproc 



