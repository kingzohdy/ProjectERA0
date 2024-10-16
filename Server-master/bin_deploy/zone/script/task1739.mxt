
proc 1
		strvar m1
		get_task_var 1739 0 8 1 m1
		var m1 0
		set_task_var 1739 0 8 @m1 1

endproc 

proc 3
	strvar ID
	GET_PLAYER_ID 1 ID
	TARGET_ADD_STATUS 1 @ID 1219 2 0 1
endproc 
proc 4
	strvar ID
	GET_PLAYER_ID 1 ID
	TARGET_ADD_STATUS 1 @ID 1219 2 0 1

endproc 
proc 5
	strvar ID
	GET_PLAYER_ID 1 ID
	TARGET_ADD_STATUS 1 @ID 1219 2 0 1


endproc 


proc 6
	strvar m1 x1 y1 a
	get_task_var 1739 0 8 1 m1
	if m1 = 1
		get_player_mapinfo 0 1 2 x1 y1
		get_point_dist @x1 @y1 21000 25100 a
		if @a < 500
			task_fini 1739 0
		endif
	endif
;		get_task_var 1717 1 8 1 30
;		add 30 1
;		set_task_var 1717 1 8 @30 1
;		if 30 = 60
;			TASK_FAIL 1717 1
;			
;			add_sys_msg_start
;			add_sys_msg 黑暗笼罩的能量从你身上消失了！
;			set_sys_msg_mode 1 0 0	
;			msg_info 
;		endif	
endproc 






