;Ԫ��ʯ

PROC 1

	TASK_STAT 1333 1 20

	if 20 = 2 or 20 = 1

		add_sys_msg_start
		add_sys_msg ���� ��Ԫ��ʯ����ɢ����ǿ����������Զ�������
		set_sys_msg_mode 3 0 0
		msg_info

	endif

ENDPROC

proc 2

	TASK_STAT 1333 1 20

	if 20 = 2 or 20 = 1
			GET_PLAYER_ID 1 54
			TARGET_ADD_STATUS 1 @54 1000 99 1 53
	endif

endproc

