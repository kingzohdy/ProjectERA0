;流金沙漠传送鸣沙

proc 1
	strvar Lv
	GET_ID_PLAYER_INFO 0 8 Lv
	if Lv >= 70
		transmit 3404 4200 6200 0
	else
		ADD_SYS_MSG_START
		ADD_SYS_MSG 此地图太危险了，为了您的身家性命，还是等70级以后再去吧！
		SET_SYS_MSG_MODE 3 0 0
		MSG_INFO
	endif
endproc 
