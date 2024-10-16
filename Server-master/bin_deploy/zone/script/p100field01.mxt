;祭祖神殿 ,招boss逻辑区域
proc 1
;	add_sys_msg_start
;	add_sys_msg 逻辑区
;	set_sys_msg_mode 4 0 0
;	map_msg_info
	;逻辑区域1标记,0未触发过,1触发过
	get_pworld_var 28 32 1 17
	if 17 = 1
		return
	endif
	VAR 17 1
	set_pworld_var 28 32 @17 1
	;招boss
	new_mon 263005 1 17675 5007 0 0 0 1 2
endproc
