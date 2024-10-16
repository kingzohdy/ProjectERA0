;权杖 
proc 1
	STRVAR m1 x1 y1 a
	var m1 0
	set_task_var 1717 0 8 @m1 1
	var 30 0
	set_task_var 1717 1 8 @30 1
endproc
proc 3
	strvar ID
	GET_PLAYER_ID 1 ID
	TARGET_ADD_STATUS 1 @ID 1229 1 0 1
endproc 
proc 4
	strvar ID
	GET_PLAYER_ID 1 ID
	TARGET_ADD_STATUS 1 @ID 1229 1 0 1

endproc 
proc 5
	strvar ID
	GET_PLAYER_ID 1 ID
	TARGET_ADD_STATUS 1 @ID 1229 1 0 1


endproc 



proc 6

	get_task_var 1717 0 8 1 m1
	if m1 = 1
		get_player_mapinfo 0 1 2 x1 y1
		get_point_dist @x1 @y1 12400 17500 a
		if @a < 1000
			task_fini 1717 0
		endif
		
		get_task_var 1717 1 8 1 30
		add 30 1
		set_task_var 1717 1 8 @30 1
		if 30 = 60
			TASK_FAIL 1717 1
			
			add_sys_msg_start
			add_sys_msg 黑暗笼罩的能量从你身上消失了！
			set_sys_msg_mode 1 0 0	
			msg_info 
		endif	
			
	endif

	set_task_var 1717 0 8 @m1 1

	
endproc

