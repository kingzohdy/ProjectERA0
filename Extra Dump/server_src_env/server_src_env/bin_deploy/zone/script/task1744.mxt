proc 1
	STRVAR playerid
ENDPROC
proc 3
	effect 2 0 stopGray()
	GET_PLAYER_ID 1 playerid
	TARGET_ADD_STATUS 1 @playerid 1018 1 1 1
ENDPROC
proc 4
	effect 2 0 stopGray()
	GET_PLAYER_ID 1 playerid
	TARGET_ADD_STATUS 1 @playerid 1018 1 1 1
ENDPROC
proc 5
	effect 2 0 stopGray()
	GET_PLAYER_ID 1 playerid
	TARGET_ADD_STATUS 1 @playerid 1018 1 1 1
ENDPROC

