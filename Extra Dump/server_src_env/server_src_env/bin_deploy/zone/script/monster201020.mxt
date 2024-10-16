;祭祖神殿 ,队长3boss
proc 1
	strvar memid
	GET_MON_INFO 1 0 memid 0 0 0 0 0
	;TARGET_ADD_STATUS 3 @memid 3035 1 25920000 0 
	TARGET_ADD_STATUS 3 @memid 1095 1 25920000 0 
	
	var 10 0
	set_mon_var 0 32 @10 0
endproc
;死亡事件

proc 4
	is_mon_fighting 1 2
	if 2 < 1
		return
	endif
	get_mon_var 0 32 0 10
	if 10 > 0
		return
	endif
	var 10 1
	set_mon_var 0 32 @10 0
	strvar ret defid memid hp mapdef map x y
	GET_MON_INFO ret defid memid hp mapdef map x y
	
	MON_SPEECH  @memid 1 2 0 有入侵者！别让他们逃了！
;	add_sys_msg_start
;	add_sys_msg 击败撕裂者基尔里斯解除暗影天幕，然后将暗影结界破坏 
;	set_sys_msg_mode 1 0 0
;	map_msg_info
endproc
