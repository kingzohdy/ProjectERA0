;祭祖神殿,巡逻兵队长
;出生事件
proc 1
	;招小弟计数
	var 10 0
	set_mon_var 0 32 @10 1
endproc

;定时事件
proc 3
	;招小弟计数
	VAR 10 0
	get_mon_var 0 32 1 10
	if 10 > 0
		return
	endif
	
	IS_MON_FIGHTING 1 11
	if 11 = 0
		return
	endif
	
	var 10 1
	set_mon_var 0 32 @10 1
	
	get_mon_group 1 12
	strvar x y
	GET_MON_INFO 1 0 0 0 0 0 x y

	new_mon 201015 1 @x @y 300 0 0 1 2
	SET_MON_GROUP 1 @2 @12
	new_mon 201016 1 @x @y 300 0 0 1 2
	SET_MON_GROUP 1 @2 @12
	
endproc

