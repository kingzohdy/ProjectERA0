

;;;;60级BOSS 触发怪
proc 1
	 strvar mid
	 GET_MON_INFO 0 0 mid 0 0 0 0 0
	 TARGET_ADD_STATUS 3 @mid 1095 1 25920000 1
	
	; get_map_val 5 8 1 60	
	; if 60 = 0
		; strvar tim
		; GET_TIME_SEC tim
		; set_map_val 1 32 @tim 1
		
		; var 60 1
		; set_map_val 5 8 @60 1
	; endif
	
endproc 

proc 2

	; strvar radmon
	; var radmon 0
	; rand radmon 100
	; if radmon < 15	
		
		; get_event_var_by_id 47 4 32 12 20
		; if 12 = 0 
			; var 20 1
			; set_event_var_by_id 47 4 32 @20 12
		; endif
		
	; endif
	
	;;;;;;;;;;;;;;;;;;;;;;;
;	add_sys_msg_start
;	add_sys_msg 随机数$@radmon$
;	set_sys_msg_mode 1 0 0
;	map_msg_info
	
	strvar tim
	get_map_val 0 8 1 50	
	get_map_val 1 32 1 tim
	get_event_var_by_id 47 4 32 12 20
	strvar tim2
	GET_TIME_SEC tim2
	sub tim2 @tim
	if tim2 >= 86400 and 50 = 0 and 20 != 1
;	if tim2 >= 100 and 50 = 0 and 20 != 1
;	if tim2 >= 60 and 50 = 0

		var 20 1
		set_event_var_by_id 47 4 32 @20 12
	endif
	
endproc  








