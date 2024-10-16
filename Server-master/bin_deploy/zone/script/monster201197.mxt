;3号boss   牧树人1

proc 2
	strvar hp
	GET_MON_INFO 0 0 0 hp 0 0 0 0
	if hp = 0
		get_pworld_var 106 8 1 46
		add 46 1
		set_pworld_var 106 8 @46 1
		if 46 = 2
			get_pworld_var 8 32 1 10
			DEL_ID_MON @10 1
			
		;	add_sys_msg_start
		;	add_sys_msg 哦，天啊！无情的时间夺去了蓝鹰队员的生命，英雄们不要放弃，去打败古武族首领，为他们报仇！
		;	set_sys_msg_mode 4 0 0
		;	map_msg_info
		else
		;	add_sys_msg_start
		;	add_sys_msg 牧树人挂了~~~~~~~~~~~~
		;	set_sys_msg_mode 4 0 0
		;	map_msg_info
		endif
	endif
endproc 

proc 9

		get_pworld_var 106 8 1 46
		var 46 0
		set_pworld_var 106 8 @46 1
;	DESTROY_MON 1
;	strvar momid
;	NEW_MON_BY_DIR 201057 1 26200 23100 0 0 0 30 1 momid
;	TARGET_ADD_STATUS 3 @momid 1095 1 25920000 0 
;	NEW_MON_BY_DIR 201058 1 26100 23600 0 0 0 30 1 momid
;	TARGET_ADD_STATUS 3 @momid 1095 1 25920000 0 

endproc 

