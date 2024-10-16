;电锯惊魂

proc 2
	get_mon_info 1 0 0 0 11 0 0 0
	get_pworld_var 28 32 1 10
	;add_sys_msg_start
	;add_sys_msg 11=$@11$ 10=$@10$
	;set_sys_msg_mode 4 0 0
	;map_msg_info

	if @10 = 0 and ( @11 = 20600 or @11 = 31300 )
	add_sys_msg_start
	add_sys_msg 身手敏捷的$_player$通过考验,电锯机关被移除了
	set_sys_msg_mode 4 0 0
	map_msg_info
	;del_id_mon 290008 1
	var 10 1
	set_pworld_var 28 32 @10 1

	endif
endproc

