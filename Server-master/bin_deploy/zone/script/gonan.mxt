proc 1
;	strvar Lv
;	GET_ID_PLAYER_INFO 0 8 Lv
;	if Lv >= 40
		transmit 2500 29600 22400 1
;	else
;		ADD_SYS_MSG_START
;		ADD_SYS_MSG 40级后才可传送到越雾雨林！
;		SET_SYS_MSG_MODE 1 0 0
;		MSG_INFO
;	endif
endproc
