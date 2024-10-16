;猎犬

proc 1

	GET_MON_INFO 1 2 30 31 32 33 0 0
	TARGET_ADD_STATUS 1 @30 1006 9 5 1
	MON_SPEECH @30 1 1 1 我感觉到目标就在不远处的三号矿坑，跟紧我！
	var 10 0
	set_mon_var 0 8 @10 1
	
endproc

proc 3
;巡逻结束
	DESTROY_MON 1

endproc 

proc 4
	get_mon_var 0 8 1 10
	add 10 1
	set_mon_var 0 8 @10 1
	
	GET_MON_INFO 1 2 30 31 32 33 0 0

	if 10 = 3
;		SET_MON_BUFF 1009 2 4 1
	
	endif
	if 10 = 7
		SET_MON_BUFF 1009 2 6 1
		
	endif
	if 10 = 11
	
		SET_MON_BUFF 1009 2 6 1
	endif
	if 10 = 15
	
		SET_MON_BUFF 1009 2 6 1
	endif
	
endproc
