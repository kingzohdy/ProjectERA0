
proc 1

	strvar Loc
	var Loc 0
	set_task_var 3030 1 8 @Loc 1

endproc

proc 6

	get_task_var 3030 1 8 1 Loc
	
	if Loc = 0
		return
	endif
	;没使用坐骑的时候
	
	strvar X Y M Map
	GET_PLAYER_MAPINFO 1 Map 1 X Y
	GET_POINT_DIST 3400 5500 @X @Y M
	if M <= 10
		DEL_ROLE_TMP_ITEM 0 0
		transmit 1000 32800 48500 1
;		strvar Mid
;		GET_PLAYER_ID 1 Mid
;		TARGET_ADD_STATUS 1 @Mid 1033 1 0 1

	endif
;;;;;;;;;;;;;;;;;;;;

;;;;;;;;;;;;;;;;;;;;

	strvar So
	get_task_var 3030 0 8 1 So
	if So = 0
		if Map = 1000 
	
			NEW_OWNER_TMP_ITEM 1460049 32800 48500 0 0 0 185 1095 1 36000 1
			var So 1
			set_task_var 3030 0 8 @So 1
		endif
	endif
endproc 

