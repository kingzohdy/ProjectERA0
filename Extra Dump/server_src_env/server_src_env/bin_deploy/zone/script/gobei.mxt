;天空岛传送落日荒野

proc 1
	strvar Lv
	GET_ID_PLAYER_INFO 0 8 Lv
	if Lv >= 15
		transmit 2400 3100 3200 1
	else
		ADD_SYS_MSG_START
		ADD_SYS_MSG 15级后才可传送到落日荒野！
		SET_SYS_MSG_MODE 1 0 0
		MSG_INFO
	endif
endproc
