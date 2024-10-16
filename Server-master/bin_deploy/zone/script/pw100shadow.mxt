;祭祖神殿 ,暗影屏障区域
proc 1
	;shadow29,暗影屏障提示
	var 29 0
	get_pworld_var 76 32 1 29
	if 29 > 0
		return 
	endif
	;shadow29,暗影屏障提示
	var 29 1
	set_pworld_var 76 32 @29 1
	add_sys_msg_start
	add_sys_msg 必须先破坏火焰结界与暗影结界，才能解除暗影屏障 
	set_sys_msg_mode 1 0 0
	map_msg_info
endproc
