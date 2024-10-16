;警报机器人  284015
proc 1

	var 10 0
	set_mon_var 0 32 @10 1

endproc


proc 4
	get_mon_var 0 32 1 10
	add 10 1
	set_mon_var 0 32 @10 1
	
	;12秒召唤小机器人
	if 10 = 4
				ADD_SYS_MSG_START
				ADD_SYS_MSG 警报机器人正在召唤它的同伴。
				SET_SYS_MSG_MODE 4 0 0
				MAP_MSG_INFO
	endif

	if 10 = 6
		get_mon_info 1 206341 0 0 0 0 100 101
		NEW_LIFE_MON 206343 2 @100 @101 300 0 0 0 60 0 1
	endif

endproc