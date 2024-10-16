;祭祖神殿 ,黑暗魔法阵3
proc 1
	var 10 0
	set_mon_var 0 32 @10 0
endproc
proc 2
	;boss4的实例id
;	VAR 15 0
;	get_pworld_var 20 32 1 15
;	
;	;1秒状态顶替掉
;	TARGET_ADD_STATUS 3 @15 3030 1 1 0	
;	;地图加状态
;	RANGE_ADD_STATUS 0 3033 1 7200 0
		;灵魂流开关计数
	get_pworld_var 8 32 1 12
	add 12 1
	set_pworld_var 8 32 @12 1
	strvar ret num
	var ret 0
	GET_MON_NUM_BY_DEFID 294028 num
	add ret @num
	GET_MON_NUM_BY_DEFID 294029 num
	add ret @num
	GET_MON_NUM_BY_DEFID 294030 num
	add ret @num
	
	if ret = 1
		add_sys_msg_start
		add_sys_msg 暗影屏障已解除，请火速前往占领黑暗祭坛! 
		set_sys_msg_mode 1 0 0
		map_msg_info
	endif
	if ret = 2
		add_sys_msg_start
		add_sys_msg 暗影屏障已削弱两层，把剩下的结界破坏将完全解除暗影屏障 
		set_sys_msg_mode 1 0 0
		map_msg_info
	endif
	if ret = 3
		add_sys_msg_start
		add_sys_msg 暗影屏障已削弱一层，把另外两个结界破坏将完全解除暗影屏障 
		set_sys_msg_mode 1 0 0
		map_msg_info
	endif
	
	
endproc
proc 4
	get_mon_var 0 32 0 10
	if 10 > 2
		return
	endif
	add 10 1
	set_mon_var 0 32 @10 0
;	add_sys_msg_start
;	add_sys_msg 暗影结界的暗影天幕已被解除 
;	set_sys_msg_mode 1 0 0
;	map_msg_info
endproc
