;302202	�Ͷ�


proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b�ټ�̵�Ҫ����Ҳ���Ǽ᲻�ɴݡ�
	add_talk #b#b�������Ĵݻ������޴�ĸ���Ҫ�������أ���
	
	strvar zhuangtai
	TASK_STAT 3103 0 zhuangtai
	
	if zhuangtai = 2
		OPTION 200 #���æ���£��Ұ����ʰ����ˣ�
	endif
	
	TASK_ADD_OPTION 3103 0 100
	
	talk
endproc

proc 100
	task_do_option 3103 1
endproc

proc 200
	strvar ret exp lv ret2 memID ret3 buffLV
	GET_PLAYER_ID 0 memID
	GET_ID_PLAYER_STATUS_INFO @memID 1330 ret buffLV 0 0 0
	if ret = 0
		TASK_FINI 3103 0
	else
		ADD_SYS_MSG_START
		ADD_SYS_MSG ���ƺ�û�а������ʻ�������ʱ�޷���ɡ��������ʡ�����
		SET_SYS_MSG_MODE 3 0 0
		MSG_INFO
	endif
endproc
