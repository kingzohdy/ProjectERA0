;302217	����


proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b�ƿ��˹ؼ��鱨�����ƿ���ʤ���Ĵ��š�
	
	strvar zhuangtai
	TASK_STAT 3722 0 zhuangtai
	if zhuangtai = 2
		option 100 #�����������鱨�ܺ���
	endif
	
	talk
endproc

proc 100
	strvar ret num
	GET_PACK_SPACE 1 num
	if num > 0
		ADD_BIND_ITEM_NUM_INPAK 5301112 1 1 ret
		if ret = 0
			TASK_FINI 3722 0
			
			ADD_SYS_MSG_START
			ADD_SYS_MSG ��ɹ��������鱨�ܺ���˳����ɡ������鱨������
			SET_SYS_MSG_MODE 3 0 0
			MSG_INFO
		endif
	else
		ADD_SYS_MSG_START
		ADD_SYS_MSG ��ı���װ���¸�����Ʒ����ʱ�޷������鱨�ܺ�
		SET_SYS_MSG_MODE 3 0 0
		MSG_INFO
	endif
endproc
