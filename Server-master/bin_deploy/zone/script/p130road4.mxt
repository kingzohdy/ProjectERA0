;70副本,走廊招怪区域4
PROC 1
	is_pworld_over 1
	if 1 = 1
		return
	endif
	strvar val
        GET_PWORLD_VAR 60 32 1 val
        If val != 0
		return
	endif
	new_mon 205170 1 19900 26700 0 0 0 1 0
	new_mon 205170 1 19600 26700 0 0 0 1 0
	new_mon 205170 1 19500 26700 0 0 0 1 0
	
	add_sys_msg_start
	add_sys_msg 警报，敌方有援军到来
	set_sys_msg_mode 3 0 0
	map_msg_info
	;逻辑区域road4标记,0未触发过,1触发过
	VAR 25 1
	set_pworld_var 60 32 @25 1
ENDPROC
