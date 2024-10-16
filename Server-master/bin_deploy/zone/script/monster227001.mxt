;试炼傀儡

proc 2
	get_pworld_var 4 8 1 31
	add 31 1
	set_pworld_var 4 8 @31 1
	if 31 = 3
		add_sys_msg_start
		add_sys_msg 第一阶段试炼完成，找艾斯进行第二段试炼吧！
		set_sys_msg_mode 1 0 0
		map_msg_info	
		
	
		GET_MAP_NPC_ID 302058 50 0 
		mon_speech @50 0 2 1 不管怎么样，你算是通过了这轮试炼，希望第二轮试炼你也能得心应手。
		EFFECT_ID 0 @50 stopMontion(%d,"294005")
		
	endif

endproc 

proc 3
;路径结束
	get_pworld_var 0 32 1 30
	add 30 1
	set_pworld_var 0 32 @30 1
	
	DESTROY_MON 1

endproc 



