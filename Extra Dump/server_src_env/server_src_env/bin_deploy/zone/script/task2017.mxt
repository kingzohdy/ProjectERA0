;;;;¿­Ðý

proc 4
	DEL_ROLE_TMP_ITEM 0 1

endproc 
proc 5

	DEL_ROLE_TMP_ITEM 0 1
endproc

proc 6
	strvar X Y M
	GET_PLAYER_MAPINFO 0 0 0 X Y
	GET_POINT_DIST @X @Y 5700 6100 M
	if M < 1
		task_fini 2017 1
		

		DEL_ROLE_TMP_ITEM 0 1
		strvar Mid
		GET_PLAYER_ID 1 Mid
		TARGET_ADD_STATUS 1 @Mid 1033 1 0 1
	
	endif
endproc 
