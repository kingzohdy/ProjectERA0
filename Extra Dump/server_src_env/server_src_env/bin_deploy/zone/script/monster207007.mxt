
proc 1
	strvar memid
	GET_MON_INFO 1 0 memid 0 0 0 0 0
	TARGET_ADD_STATUS 3 @memid 1095 1 25920000 0 

endproc 

proc 2
	strvar ret xianzhi
	
	rand 10 10000
	if 10 < 5
		CHECK_WORLD_LIMIT 5611417 xianzhi
		MON_DROP_BOOTYGROUP 5611417 0 0 0 ret

		if ret = 0 and xianzhi != -1
			add_sys_msg_start
			add_sys_msg 年兽被勇士成功的击败，掉落了1个“变异年兽·马”稀有宠物！
			set_sys_msg_mode 18 0 0
			svr_msg_info
		endif
	endif
	

;	get_event_var_by_id 75 0 32 1 20
;	var 20 1
;	set_event_var_by_id 75 0 32 @20 1
	
	
endproc 

