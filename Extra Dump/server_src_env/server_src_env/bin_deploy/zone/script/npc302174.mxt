;npc ����

proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b������գ�׳��ɽ�ӣ���ʿ���֣�����ط�Բ������ͬ�콭�Ӹ質����������������ȶ��������Ҹ����������������¡���
	add_talk #b#b
	add_talk #b#b�ڴ˹���ѽ�֮�ʣ��ҽ�����ʱ���������λ��ʿ�������ֿ��ף�������ͣ�
	
	GET_PLAYER_BASE_ATT 13 0 12 0
	if @13 = 0 and @12 >= 40
		option 200 ��ȡ���ߴ���
	elseif @13 = 0 and @12 < 40 
		GET_PLAYER_DAILY_FLAG 0 15 22
		if @22 = 0
			option 101 ��ȡ������
		endif
	else
	endif
		
	talk
endproc 

proc 101
	GET_PLAYER_DAILY_FLAG 0 15 11
	;0��ʾδ����
	if 11 = 1
		PLAYER_CLOSE_DIALOG1 0 0
		return
	endif
	
	strvar ret1
	GET_PACK_SPACE 0 ret1
	if @ret1 = 0
		add_sys_msg_start
		add_sys_msg �����ռ䲻�㣬�޷���ȡ
		set_sys_msg_mode 4 0 0
		msg_info
		return
	endif

	strvar Ret 
	SET_PLAYER_DAILY_FLAG Ret 15 1
	if Ret = 0
		ADD_BIND_ITEM_NUM_INPAK 5012117 5 1 14 
		add_sys_msg_start
		add_sys_msg ����ڿ��֣�
		set_sys_msg_mode 3 0 0
		msg_info
	endif
	PLAYER_CLOSE_DIALOG1 0 0
endproc

proc 200
	GET_DAYONLINE 1 10
	
	var 11 @10
	if 10 >= 3600
		dev 10 3600
		MOD 11 3600
		dev 11 60
	else
		var 10 0
		dev 11 60
	endif
	
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b ÿ������6����������ʱ�䣬��������ʱ����ۼơ�
	add_talk #b#b
	add_talk #b#b�����������ʱ��Ϊ��$@10$ʱ$@11$��
	
	GET_PLAYER_DAILY_FLAG 0 10 20
	GET_PLAYER_DAILY_FLAG 0 11 21
	GET_PLAYER_DAILY_FLAG 0 13 22
	
	if @20 = 0
		option 201 ��ȡ������
	endif
	
	if @21 = 0
		option 202 ��ȡ������2Сʱ����
	endif
	
	if @22 = 0
		option 203 ��ȡ������5Сʱ����
	endif
		
	talk
endproc

proc 201
	GET_PLAYER_DAILY_FLAG 0 10 11
	;0��ʾδ����
	if 11 = 1
		PLAYER_CLOSE_DIALOG1 0 0
		return
	endif
	
	strvar ret1
	GET_PACK_SPACE 0 ret1
	if @ret1 < 2
		add_sys_msg_start
		add_sys_msg ����Ϊ��׼������������Ӵ����������Ԥ��2���ո���������ȡ�ɣ�
		set_sys_msg_mode 4 0 0
		msg_info
		return
	endif

	strvar Ret 
	SET_PLAYER_DAILY_FLAG Ret 10 1
	if Ret = 0
		ADD_BIND_ITEM_NUM_INPAK 5060047 10 1 14 
		ADD_BIND_ITEM_NUM_INPAK 5060050 10 1 15 
	endif
	PLAYER_CLOSE_DIALOG1 0 0
endproc

proc 202
	GET_DAYONLINE 1 10
	
	GET_PLAYER_DAILY_FLAG 0 11 19
	;0��ʾδ����
	if 19 = 1
		PLAYER_CLOSE_DIALOG1 0 0
		return
	endif
	
	if 10 < 7200
		add_sys_msg_start
		add_sys_msg ��Ǹ������������ʱ�䲻��2Сʱ�����Ժ�������ȡ��
		set_sys_msg_mode 4 0 0
		msg_info
		return
	else
		strvar ret1
		GET_PACK_SPACE 0 ret1
		if @ret1 < 2
			add_sys_msg_start
			add_sys_msg ����Ϊ��׼������������Ӵ����������Ԥ��2���ո���������ȡ�ɣ�
			set_sys_msg_mode 4 0 0
			msg_info
			return
		endif
	
		strvar Ret 
		SET_PLAYER_DAILY_FLAG Ret 11 1
		if Ret = 0
			ADD_BIND_ITEM_NUM_INPAK 5060047 10 1 14 
			ADD_BIND_ITEM_NUM_INPAK 5060050 30 1 15 
		endif
		PLAYER_CLOSE_DIALOG1 0 0
	endif
endproc

proc 203
	GET_DAYONLINE 1 10
	
	GET_PLAYER_DAILY_FLAG 0 13 11
	;0��ʾδ����
	if 11 = 1
		PLAYER_CLOSE_DIALOG1 0 0
		return
	endif
	
	if 10 < 18000
		add_sys_msg_start
		add_sys_msg ��Ǹ������������ʱ�䲻��5Сʱ�����Ժ�������ȡ��
		set_sys_msg_mode 4 0 0
		msg_info
		return
	else
		strvar ret1
		GET_PACK_SPACE 0 ret1
		if @ret1 < 2
			add_sys_msg_start
			add_sys_msg ����Ϊ��׼������������Ӵ����������Ԥ��2���ո���������ȡ�ɣ�
			set_sys_msg_mode 4 0 0
			msg_info
			return
		endif
	
		strvar Ret 
		SET_PLAYER_DAILY_FLAG Ret 13 1
		if Ret = 0
			ADD_BIND_ITEM_NUM_INPAK 5060047 10 1 14 
			ADD_BIND_ITEM_NUM_INPAK 5060050 60 1 15
		endif
		PLAYER_CLOSE_DIALOG1 0 0
	endif
endproc
