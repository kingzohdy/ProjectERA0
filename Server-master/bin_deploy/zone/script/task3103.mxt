;��������

Proc 3
	strvar ret exp lv ret2 memID ret3 buffLV
	GET_PLAYER_ID 0 memID
	GET_ID_PLAYER_STATUS_INFO @memID 1330 ret buffLV 0 0 0
	if ret = 0
		if buffLV = 1
			TARGET_ADD_STATUS 1 @memID 1330 1 1 ret3
			if ret3 = 0
			
				;ϵ��*contributeEXP/10000
				GET_ID_PLAYER_INFO 0 19 exp
				GET_PLAYER_BASE_ATT 0 0 lv 0
				dev @exp 10000
				mul @exp 90
				mul @exp 5
				ADD_EXP 0 @exp ret2
				
				if ret2 = 0
					ADD_SYS_MSG_START
					ADD_SYS_MSG ��ɹ�����һ������ͨ���ʡ�������˴�������ֵ��
					SET_SYS_MSG_MODE 4 0 0
					MSG_INFO
					
					TASK_FINI 3103 0
				endif
			else
				ADD_SYS_MSG_START
				ADD_SYS_MSG �������޷��ݽ����ʣ����Ժ�������
				SET_SYS_MSG_MODE 3 0 0
				MSG_INFO
			endif
		endif
		
		if buffLV = 2
			TARGET_ADD_STATUS 1 @memID 1330 2 1 ret3
			if ret3 = 0
			
				;ϵ��*contributeEXP/10000
				GET_ID_PLAYER_INFO 0 19 exp
				GET_PLAYER_BASE_ATT 0 0 lv 0
				dev @exp 10000
				mul @exp 90
				mul @exp 15
				ADD_EXP 0 @exp ret2
				
				if ret2 = 0
					ADD_SYS_MSG_START
					ADD_SYS_MSG ��ɹ�����һ�����������ʡ�������˴�������ֵ��
					SET_SYS_MSG_MODE 4 0 0
					MSG_INFO
					
					TASK_FINI 3103 0
				endif
			else
				ADD_SYS_MSG_START
				ADD_SYS_MSG �������޷��ݽ����ʣ����Ժ�������
				SET_SYS_MSG_MODE 3 0 0
				MSG_INFO
			endif
		endif
		
		if buffLV = 3
			TARGET_ADD_STATUS 1 @memID 1330 3 1 ret3
			if ret3 = 0
			
				;ϵ��*contributeEXP/10000
				GET_ID_PLAYER_INFO 0 19 exp
				GET_PLAYER_BASE_ATT 0 0 lv 0
				dev @exp 10000
				mul @exp 90
				mul @exp 30
				ADD_EXP 0 @exp ret2
				
				if ret2 = 0
					ADD_SYS_MSG_START
					ADD_SYS_MSG ��ɹ�����һ�����������ʡ�������˴�������ֵ��
					SET_SYS_MSG_MODE 4 0 0
					MSG_INFO
					
					TASK_FINI 3103 0
				endif
			else
				ADD_SYS_MSG_START
				ADD_SYS_MSG �������޷��ݽ����ʣ����Ժ�������
				SET_SYS_MSG_MODE 3 0 0
				MSG_INFO
			endif
		endif
	else
		ADD_SYS_MSG_START
		ADD_SYS_MSG ���ƺ�û�а������ʻ�������ʱ�޷���ɡ��������ʡ�����
		SET_SYS_MSG_MODE 3 0 0
		MSG_INFO
	endif
endproc

proc 4
	strvar memID ret buffLV
	GET_PLAYER_ID 0 memID
	GET_ID_PLAYER_STATUS_INFO @memID 1330 ret buffLV 0 0 0
	if ret = 0
		if buffLV = 1
			TARGET_ADD_STATUS 1 @memID 1330 1 1 0
		endif
		if buffLV = 2
			TARGET_ADD_STATUS 1 @memID 1330 2 1 0
		endif
		if buffLV = 3
			TARGET_ADD_STATUS 1 @memID 1330 3 1 0
		endif
	endif
endproc

proc 5
	strvar memID ret buffLV
	GET_PLAYER_ID 0 memID
	GET_ID_PLAYER_STATUS_INFO @memID 1330 ret buffLV 0 0 0
	if ret = 0
		if buffLV = 1
			TARGET_ADD_STATUS 1 @memID 1330 1 1 0
		endif
		if buffLV = 2
			TARGET_ADD_STATUS 1 @memID 1330 2 1 0
		endif
		if buffLV = 3
			TARGET_ADD_STATUS 1 @memID 1330 3 1 0
		endif
	endif
endproc
