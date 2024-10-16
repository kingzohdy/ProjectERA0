;70副本,第4个变身后的警报器
proc 1
	call 100
endproc
proc 4
	call 100
endproc
proc 100
	;逻辑区域boss1触发区,0未触发过,1第一排报警,2第一排报警,3第三排报警
	get_pworld_var 80 32 1 30
	if 30 < 1
		return
	endif
	strvar x y num num2
	GET_MON_INFO 1 0 0 0 0 0 x y
	GET_MON_NUM_BY_DEFID 206313 num
	if num < 0
		return
	endif
	if num < 1
		new_mon 206313 1 @x @y 0 0 0 1 0
	endif
	GET_MON_NUM_BY_DEFID 206319 num2
	if num2 < 0
		return
	endif
	if num2 < 1
		new_mon 206319 1 @x @y 0 0 0 1 0
	endif
	if num < 1 or num2 < 1
		add_sys_msg_start
		add_sys_msg 警报！警报！发现入侵者！
		set_sys_msg_mode 1 0 0
		map_msg_info
	endif
endproc
