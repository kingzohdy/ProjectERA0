
proc 1
	strvar sta1
	IS_IN_PWORLD  515 20 sta1 0
	if @sta1 = 1 and @20 = 0
		set_pworld_var 0 32 0 1
		set_pworld_var 8 32 0 1
		set_pworld_var 4 32 0 1
		DAQIANGDAO_FENSHU 0 0
	endif
endproc

proc 2
	strvar sta1
	IS_IN_PWORLD  515 20 sta1 0
	if @sta1 = 1 and @20 = 0
		DELETE_MON_BY_DEFID 230059 0
		DELETE_MON_BY_DEFID 230060 0
		DELETE_MON_BY_DEFID 230061 0
		DELETE_MON_BY_DEFID 230062 0
		DELETE_MON_BY_DEFID 230063 0	
	endif
	add_sys_msg_start
	add_sys_msg 任务已完成，可通过传送门离开
	set_sys_msg_mode 4 0 0
	msg_info
endproc

;TASK_FAIL
proc 4
	strvar sta1
	IS_IN_PWORLD  515 20 sta1 0
	if @sta1 = 1 and @20 = 0
		DELETE_MON_BY_DEFID 230059 0
		DELETE_MON_BY_DEFID 230060 0
		DELETE_MON_BY_DEFID 230061 0
		DELETE_MON_BY_DEFID 230062 0
		DELETE_MON_BY_DEFID 230063 0	
		
		set_pworld_var 0 32 0 1
		set_pworld_var 4 32 0 1
	endif
endproc

;TASK_DROP
proc 5
	strvar sta1
	IS_IN_PWORLD  515 20 sta1 0
	if @sta1 = 1 and @20 = 0
		DELETE_MON_BY_DEFID 230059 0
		DELETE_MON_BY_DEFID 230060 0
		DELETE_MON_BY_DEFID 230061 0
		DELETE_MON_BY_DEFID 230062 0
		DELETE_MON_BY_DEFID 230063 0	
		
		set_pworld_var 0 32 0 1
		set_pworld_var 4 32 0 1
	endif
endproc

