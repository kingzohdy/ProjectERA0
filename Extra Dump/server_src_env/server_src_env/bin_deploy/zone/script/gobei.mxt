;��յ��������ջ�Ұ

proc 1
	strvar Lv
	GET_ID_PLAYER_INFO 0 8 Lv
	if Lv >= 15
		transmit 2400 3100 3200 1
	else
		ADD_SYS_MSG_START
		ADD_SYS_MSG 15����ſɴ��͵����ջ�Ұ��
		SET_SYS_MSG_MODE 1 0 0
		MSG_INFO
	endif
endproc
