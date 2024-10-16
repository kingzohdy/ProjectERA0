;70副本,走廊逻辑区提示
proc 1
	is_pworld_over 1
	if 1 = 1
		return
	endif
	;roadtipflag31,走廊提示逻辑区标记,0未触发过,1触发
	var 31 0
	get_pworld_var 84 32 1 31
	if 31 > 0
		return
	endif
	add 31 1
	;roadtipflag31,走廊提示逻辑区标记,0未触发过,1触发
	var 31 @31;
	set_pworld_var 84 32 @31 1
	add_sys_msg_start
	add_sys_msg 小心，这里处处透露着危险的气息 
	set_sys_msg_mode 3 0 0
	map_msg_info
endproc
