;牧牛任务,队伍牧牛
;;队伍牧牛
;;死亡
proc 2
;	SET_MON_TASK_STAT 2000 2 1
endproc
;;tick
;proc 4
;	 MON_TASK_AREA_CHECK 2000 3000 300 1
;	 if 1 = 0
;		del_mon 1
;	 endif
;endproc
;;强制移动到终点的事件
;proc 8
;	strvar x y dis angle
;	GET_MON_INFO 1 0 0 0 0 0 x y
;	add_sys_msg_start
;	add_sys_msg 1:$@x$:$@y$
;	set_sys_msg_mode 4 0 0
;	mon_owner_msg_info	
;
;	GET_POINT_DIST @x @y 10187 19213 dis
;		
;	if @dis <= 600
;		SET_MON_TASK_STAT 2000 1 1
;		DESTROY_MON 1
;		return
;	endif
;	if @dis <= 500
;		return
;	endif
;	add_sys_msg_start
;	add_sys_msg 牧牛喘了口气
;	set_sys_msg_mode 4 0 0
;	mon_owner_msg_info	
;
;	get_radian 1 @x @y 10187 19213 angle
;	get_angle_pos 1 @x @y @angle 500 x y
;		add_sys_msg_start
;	add_sys_msg 1:$@x$:$@y$
;	set_sys_msg_mode 4 0 0
;	mon_owner_msg_info
;	
;	add angle 180
;	if angle > 360
;		sub angle 360
;	endif
;
;	effect 3 0 playEffct($@x$,$@y$,$@angle$,"effect\\entity\\9570_16.ent")
;endproc
