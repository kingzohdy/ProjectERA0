proc 1
	
	dialog 1
	
	add_talk $_npc$:
	add_talk #b#b֧��������������ת����Դ����������Դ�������նˣ���ħ��ʦ�ĳ����о����֣���Դ������Զ����ˡ�
	add_talk #b#b����ԴӰ����ˣ����Զ�����Ľ��������������ǳ��˵���������˱�����ð�ռ�����������ð�յĽ��ȡ�
	add_talk #b#b#c186506����ȡ˫������״̬������100%ɱ�־���#n
	call 200
	
	OPTION 101 #��ȡ0.5Сʱ˫��ʱ�� 
	OPTION 102 #��ȡ1Сʱ˫��ʱ��
	OPTION 103 #��ȡ2Сʱ˫��ʱ��
	OPTION 104 #�鿴����ʣ��˫��ʱ�� 
	talk
endproc

proc 200
	strvar Ret1 Ret2 Ftime Times
	get_role_var 1 32 Ret1 Ftime
	if Ret1 = -1
		return
	endif
	IS_SAME_WEEK @Ftime Ret2
	;�Ƿ�ͬһ��
	if @Ret2 = -1
		var Times 0
		set_role_var 5 8 @Times Ret2
	endif
endproc

proc 101
	
	call 200
	strvar Ret1 Ret2 Ret3 Ret4
	strvar Ftime Times MemID
	;GET_ID_PLAYER_INFO #��ɫʵ��ID��0��ʾ�Լ��� #AttrFlag  #����ֵ
	GET_ID_PLAYER_INFO 0 1 MemID
	HAS_STATUS 1052 Ret4
	get_role_var 5 8 Ret2 Times
	if Ret2 = 0
		if Times < 14
;			if Ret4 = -1
			;�Ƿ���˫��״̬
				if Times = 0
					GET_TIME_SEC Ftime
					set_role_var 1 32 @Ftime Ret1
				endif
				add Times 1
				set_role_var 5 8 @Times Ret2
				TARGET_ADD_STATUS 1 @MemID 1052 1 1800 Ret3
				
				add_sys_msg_start
				add_sys_msg ���ɹ���ȡ0.5Сʱ˫������״̬��
				set_sys_msg_mode 3 0 0
				msg_info
;			else
;				add_sys_msg_start
;				add_sys_msg ������˫������״̬�����Ժ���ȡ��
;				set_sys_msg_mode 4 0 0
;				msg_info
;			endif
		else
			add_sys_msg_start
			add_sys_msg ������˫������ʱ�������꣬��ȴ����ܸ��£�
			set_sys_msg_mode 4 0 0
			msg_info
		endif

	else
		return
	endif
endproc

proc 102
	call 200
	strvar Ret1 Ret2 Ret3 Ret4
	strvar Ftime Times MemID
	;GET_ID_PLAYER_INFO #��ɫʵ��ID��0��ʾ�Լ��� #AttrFlag  #����ֵ
	GET_ID_PLAYER_INFO 0 1 MemID
	HAS_STATUS 1052 Ret4
	get_role_var 5 8 Ret2 Times
	if Ret2 = 0
		if Times <= 12
;			if Ret4 = -1
			;�Ƿ���˫��״̬
				if Times = 0
					GET_TIME_SEC Ftime
					set_role_var 1 32 @Ftime Ret1
				endif
				
				add Times 2
				set_role_var 5 8 @Times Ret2
				TARGET_ADD_STATUS 1 @MemID 1052 1 3600 Ret3
				
				add_sys_msg_start
				add_sys_msg ���ɹ���ȡ1Сʱ˫������״̬��
				set_sys_msg_mode 3 0 0
				msg_info
;			else
;				add_sys_msg_start
;				add_sys_msg ������˫������״̬�����Ժ���ȡ��
;				set_sys_msg_mode 4 0 0
;				msg_info
;			endif
		else
			if Times = 14
				add_sys_msg_start
				add_sys_msg ������˫������ʱ�������꣬��ȴ����ܸ��£�
				set_sys_msg_mode 4 0 0
				msg_info
			else
				add_sys_msg_start
				add_sys_msg ������˫������ʱ�䲻��1Сʱ���޷���ȡ��
				set_sys_msg_mode 4 0 0
				msg_info
			endif
		endif
	else
		return
	endif
endproc

proc 103

	call 200
	strvar Ret1 Ret2 Ret3 Ret4
	strvar Ftime Times MemID
	;GET_ID_PLAYER_INFO #��ɫʵ��ID��0��ʾ�Լ��� #AttrFlag  #����ֵ
	GET_ID_PLAYER_INFO 0 1 MemID
	HAS_STATUS 1052 Ret4
	get_role_var 5 8 Ret2 Times
	if Ret2 = 0
		if Times <= 10
;			if Ret4 = -1
			;�Ƿ���˫��״̬
				if Times = 0
					GET_TIME_SEC Ftime
					set_role_var 1 32 @Ftime Ret1
				endif
				
				add Times 4
				set_role_var 5 8 @Times Ret2
				TARGET_ADD_STATUS 1 @MemID 1052 1 7200 Ret3
				
				add_sys_msg_start
				add_sys_msg ���ɹ���ȡ2Сʱ˫������״̬��
				set_sys_msg_mode 3 0 0
				msg_info
;			else
;				add_sys_msg_start
;				add_sys_msg ������˫������״̬�����Ժ���ȡ��
;				set_sys_msg_mode 4 0 0
;				msg_info
;			endif
		else
			if Times = 14
				add_sys_msg_start
				add_sys_msg ������˫������ʱ�������꣬��ȴ����ܸ��£�
				set_sys_msg_mode 4 0 0
				msg_info
			else
				add_sys_msg_start
				add_sys_msg ������˫������ʱ�䲻��2Сʱ���޷���ȡ��
				set_sys_msg_mode 4 0 0
				msg_info
			endif
		endif
	else
		return
	endif
endproc

proc 104

	call 200
	strvar Ret1 Ret2 Ret3
	strvar Ftime Times Stop Ltime
	get_role_var 5 8 Ret2 Times
	if Ret2 = 0
		if Times = 0 or @Times = 2 or @Times = 4 or @Times = 6 or @Times = 8 or @Times = 10 or @Times = 12 or @Times = 14
			var Ltime 14
			sub Ltime @Times
			dev Ltime 2
					add_sys_msg_start
					add_sys_msg ������˫������ʱ��ʣ��$@Ltime$Сʱ��
					set_sys_msg_mode 3 0 0
					msg_info
		endif
		
		if Times = 1 or @Times = 3 or @Times = 5 or @Times = 7 or @Times = 9 or @Times = 11 or @Times = 13
			var Ltime 14
			sub Ltime @Times
			dev Ltime 2
					add_sys_msg_start
					add_sys_msg ������˫������ʱ��ʣ��$@Ltime$.5Сʱ��
					set_sys_msg_mode 3 0 0
					msg_info
		endif
	else
		return
	endif
endproc
