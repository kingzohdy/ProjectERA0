;����ɳĮ������ɳ

proc 1
	strvar Lv
	GET_ID_PLAYER_INFO 0 8 Lv
	if Lv >= 70
		transmit 3404 4200 6200 0
	else
		ADD_SYS_MSG_START
		ADD_SYS_MSG �˵�ͼ̫Σ���ˣ�Ϊ������������������ǵ�70���Ժ���ȥ�ɣ�
		SET_SYS_MSG_MODE 3 0 0
		MSG_INFO
	endif
endproc 
