;70副本,光能网控制器5
proc 1
	var 10 0
	set_mon_var 0 32 @10 1
endproc
proc 2
	call 100
endproc
proc 4
	call 100
endproc
proc 100
	get_mon_var 0 32 1 10
	if 10 > 0
		return
	endif
	is_mon_fighting 1 2
	if 2 < 1
		return
	endif
	var 10 1
	set_mon_var 0 32 @10 1
;	strvar ret defid memid hp mapdef map x y
;	GET_MON_INFO ret defid memid hp mapdef map x y
;	
;	new_MON 290013 2 @x @y 50 0 0 1 0
	new_MON 290013 1 9009 15332 50 0 0 1 0
	new_MON 290013 1 8250 16943 50 0 0 1 0
	new_MON 290013 1 8985 16940  50 0 0 1 0
endproc
