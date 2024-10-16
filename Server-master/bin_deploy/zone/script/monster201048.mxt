;2号结界BOSS 德鲁伊

proc 2
	strvar hp
	GET_MON_INFO 0 0 0 hp 0 0 0 0
	if hp = 0
		get_pworld_var 4 32 1 10
		DEL_ID_MON @10 1
		
		add_sys_msg_start
		add_sys_msg 时间在不停的流逝，英雄们快赶去囚牢结界吧
		set_sys_msg_mode 4 0 0
		map_msg_info
	endif
endproc 

proc 9
	DESTROY_MON 1
	strvar momid
	NEW_MON_BY_DIR 201048 1 11400 24000 0 0 0 30 1 momid	
	TARGET_ADD_STATUS 3 @momid 1095 1 25920000 0 

endproc 

