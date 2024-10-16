;70副本,走廊招怪区域5
PROC 1
	is_pworld_over 1
	if 1 = 1
		return
	endif
	strvar val
        GET_PWORLD_VAR 64 32 1 val
        If val != 0
		return
	endif
	new_mon 284069 1 19900 28000 0 0 0 1 0
	new_mon 284070 1 19700 28000 0 0 0 1 0
	new_mon 284070 1 19400 28000 0 0 0 1 0
	;逻辑区域road5标记,0未触发过,1触发过
	VAR 26 1
	set_pworld_var 64 32 @26 1
	add_sys_msg_start
	add_sys_msg 无畏的勇士，接下来，你们要先解决左边通道的麻烦！
	set_sys_msg_mode 3 0 0
	map_msg_info
	add_sys_msg_start
	add_sys_msg 我已经在这附近留下线索，其中有更详细的提示，请仔细寻找！
	set_sys_msg_mode 3 0 0
	map_msg_info
ENDPROC
