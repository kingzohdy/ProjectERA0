;战鼓村 逻辑区域1


PROC 1

	GET_PWORLD_VAR 16 32 1 14
	If 14 = 0

		    NEW_MON 201001 3 10445 16598 500 0 0 1 2
		    NEW_MON 231020 6 10445 16598 500 0 0 1 2
	            NEW_MON 284013 1 10445 16598 500 0 0 1 2

		    add_sys_msg_start
			add_sys_msg 变异的战鼓村村民冲出来保护圣战鼓。
			set_sys_msg_mode 4 0 0
			map_msg_info

		var 14 1
		set_pworld_var 16 32 @14 1

	endif


ENDPROC