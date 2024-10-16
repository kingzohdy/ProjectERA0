;70副本,走廊招怪区域2
PROC 1
	is_pworld_over 1
	if 1 = 1
		return
	endif
	strvar val
        GET_PWORLD_VAR 52 32 1 val
        If val != 0
		return
	endif
	new_mon 284070 1 19109 22303 0 0 0 1 0
	new_mon 226015 1 19414 22302 0 0 0 1 0
	new_mon 226015 1 19600 22700 0 0 0 1 0
;	new_mon 226015 1 19300 22700 0 0 0 1 0
	
	add_sys_msg_start
	add_sys_msg 有敌方目标出现，注意隐蔽
	set_sys_msg_mode 3 0 0
	map_msg_info
	;逻辑区域road2标记,0未触发过,1触发过
	VAR 23 1
	set_pworld_var 52 32 @23 1
ENDPROC
