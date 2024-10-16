

proc 6
	strvar X Y M
	GET_PLAYER_MAPINFO 1 1 1 X Y
	GET_POINT_DIST 35000 27200 @X @Y M
	if M < 150
		task_fini 1932 1
		DEL_ROLE_TMP_ITEM 0 0
		
		strvar Mid
		GET_PLAYER_ID 1 Mid
		TARGET_ADD_STATUS 1 @Mid 1033 1 0 1
	endif
endproc 
