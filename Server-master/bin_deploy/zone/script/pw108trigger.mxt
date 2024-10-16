;70副本,入口boss,警报触发区域
proc 1
	is_pworld_over 1
	if 1 = 1
		return
	endif
	;boss1警报阶段
	get_pworld_var 80 32 1 30
	if 30 > 0
		return
	endif
	var 30 1
	set_pworld_var 80 32 @30 1
	;boss1flag32,boss1召唤标记,0未触发,1以后为计时
	var 32 1
	set_pworld_var 88 32 @32 1
	add_sys_msg_start
	add_sys_msg 有外敌入侵，启动防御程序！
	set_sys_msg_mode 1 0 0
	map_msg_info
endproc
