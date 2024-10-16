proc 1
;	strvar Lv
;	GET_ID_PLAYER_INFO 0 8 Lv
;	if Lv >= 25
		transmit 2300 16000 20725 1
;	else
;		ADD_SYS_MSG_START
;		ADD_SYS_MSG 25级后才可传送到流金沙漠！
;		SET_SYS_MSG_MODE 1 0 0
;		MSG_INFO
;	endif
endproc
