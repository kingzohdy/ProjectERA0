;祭祖神殿 ,boss 触发区域
proc 1
	;shadow32,boss暗影屏障提示
	var 32 0
	get_pworld_var 88 32 1 32
	if 32 > 0
		return
	endif
	;pworld33,是否pworldover了,1副本没结束,0结束了
	var 33 0
	get_pworld_var 92 32 1 33
	if 33 = 0
		return
	endif
	;灵魂流开关计数
	get_pworld_var 8 32 1 12
	if 12 = 0
		add_sys_msg_start
		add_sys_msg 黑暗祭坛被暗影屏障保护着，请先前往三个节点破坏结界 
		set_sys_msg_mode 1 0 0
		map_msg_info
		return
	endif
	if 12 = 1
		add_sys_msg_start
		add_sys_msg 必须把另外两个结界破坏才能完全解除暗影屏障 
		set_sys_msg_mode 1 0 0
		map_msg_info
		return
	endif 
	if 12 = 2
		add_sys_msg_start
		add_sys_msg 必须把剩下的结界破坏才能完全解除暗影屏障 
		set_sys_msg_mode 1 0 0
		map_msg_info
		return
	endif
	if 12 = 3
		add_sys_msg_start
		add_sys_msg 阿萨曼已按耐不住，请坚守着黑暗祭坛！ 
		set_sys_msg_mode 1 0 0
		map_msg_info
		DELETE_MON_BY_DEFID 261058 1
		add 12 1
		set_pworld_var 8 32 @12 1	
		DELETE_MON_BY_DEFID 294031 1
		var 32 1
		;shadow32,boss暗影屏障提示
		
		set_pworld_var 88 32 @32 1
	endif
endproc
