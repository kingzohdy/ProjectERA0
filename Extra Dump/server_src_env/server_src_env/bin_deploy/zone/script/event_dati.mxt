PROC 1

	strvar LV
	GET_PLAYER_BASE_ATT 0 0 LV 0
	if LV >= 10
		ADD_SYS_MSG_START
		SET_SYS_MSG_MODE 4 0 0
		ADD_SYS_MSG ȫ�����������Ѿ�����,����������Ϸ���������������ּ��ɲμ�,���Ľ����ڵȴ�����!
		SVR_MSG_INFO
	endif
	
endproc 

