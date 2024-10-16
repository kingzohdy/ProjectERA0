;祭祖神殿 ,巡逻兵队长
;出生事件
proc 1
	;招小弟计数
	var 10 0
	set_mon_var 0 32 @10 1
endproc
proc 2
	var 10 0
	rand 10 100
	if 10 < 40
		strvar ret defid memid hp mapdef map x y
		GET_MON_INFO ret defid memid hp mapdef map x y
		
		NEW_TMP_ITEM 1420004 @x @y 0 0 0 1
	endif
endproc
;定时事件
proc 4
	;招小弟计数
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
	strvar x y mid
	GET_MON_INFO 1 0 mid 0 0 0 x y
	mon_speech @mid 2 2 1 发现入侵者！挡住他们！
endproc
