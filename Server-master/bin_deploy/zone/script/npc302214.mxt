;302214	�����


proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b��������������������������ս����ζ����
	
	strvar zhuangtai
	TASK_STAT 3720 0 zhuangtai
	if zhuangtai = 2
		option 100 #������ת�﹤���ܼ���
	endif
	
	talk
endproc

proc 100
	strvar ret num
	GET_ITEM_NUM_INPAK 5301110 0 num
	if num > 0
		DEC_ITEM_NUM_INPAK 5301110 @num ret
		if ret = 0
			TASK_FINI 3720 0
			
			ADD_SYS_MSG_START
			ADD_SYS_MSG �㽻���˹����ܼ���˳����ɡ����﹤��������
			SET_SYS_MSG_MODE 3 0 0
			MSG_INFO
		endif
	else
		ADD_SYS_MSG_START
		ADD_SYS_MSG �����Ҳ��������ܼ�����ʱ�޷���ɡ����﹤��������
		SET_SYS_MSG_MODE 3 0 0
		MSG_INFO
	endif
endproc
