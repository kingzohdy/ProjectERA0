;��ɯ
 
proc 1	
	dialog 1
	add_talk $_npc$:
	add_talk #b#b��֪ʲôʱ���ഺ���϶�׾ԣ���֪ʲôʱ����������Ȼ��������֪ʲôʱ��ͯ��Ҳ������ɢ���Һ����ң����ഺ������ȥ�ĵ��ڣ����ܺ͡�ʱ����׶�԰����С�ɰ�һ�����ͯ�����Ϸ��������ͯ��ļ��䣬���ݴ���!
	add_talk #b#b
	add_talk #b#b#cff00806��7������24��������ɯ�ͻ���ź����ǻص��׶�԰��
	add_talk #b#b
	add_talk #b#b#c186506���ֽ��������껪��õ����Ҵ������ߣ�
	
		TASK_ADD_OPTION 4446 0 100
		TASK_ADD_OPTION 4447 0 101
		TASK_ADD_OPTION 4448 0 102
	
	task_stat 4447 1 20
	GET_ITEM_NUM_INPAK 5241520 1 10
	;�����ύ
		if @20 = 3 and @10 < 1 
			option 160 �ܱ�Ǹ���Ҳ�С�İ�����Ū���ˣ�
		;elseif @20 = 2
		;	task_fini 4447 1
		;else
		endif
		
	task_stat 4448 1 30
	GET_ITEM_NUM_INPAK 5241521 1 12
		if @30 = 3 and @12 < 1 
			option 162 �ܱ�Ǹ���Ҳ�С�İ�����Ū���ˣ�
		;elseif @30 = 2
		;	task_fini 4448 1
		;else
		endif
	
	GET_PLAYER_DAILY_FLAG 0 9 21
	;0��ʾδ����
	if 21 = 0
	option 151 ��ȡ���߽���
	endif
	
	talk
	
endproc

proc 100
	TASK_DO_OPTION 4446 0
endproc
proc 101
	TASK_DO_OPTION 4447 0
endproc

proc 102
	TASK_DO_OPTION 4448 0
endproc



;��ȡ���߽��������
proc 150
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
    
	GET_ID_PLAYER_INFO 0 19 12
	DEV 12 10000
	MUL 12 50

	dialog 1
	add_talk $_npc$:
	add_talk #b#bÿ������6����������ʱ�䣬��������ʱ����ۼơ�
	add_talk #b#b�ۼ�����2Сʱ��#cff4e00����ȡ$@12$�㾭�齱��#n
	add_talk #b#b
	add_talk #b#b����������ʱ��Ϊ��$@10$ʱ$@11$��

    option 151 ��ȡ����

	option 1 ����
	talk
endproc


;��ȡ����
proc 151
	GET_DAYONLINE 1 10
	
	GET_PLAYER_DAILY_FLAG 0 9 11
	;0��ʾδ����
	if 11 = 1
		PLAYER_CLOSE_DIALOG1 0 0
		return
	endif
	
	if 10 < 7200
		dialog 1
		add_talk $_npc$:
		add_talk #b#b�Բ�������������ʱ�䲻��2Сʱ��������ȡ������
		option 1 ����
		talk
	else
		strvar Ret 
		SET_PLAYER_DAILY_FLAG Ret 9 1
		if Ret = 0
			GET_ID_PLAYER_INFO 0 19 13
			DEV 13 10000
			MUL 13 50
			
			ADD_EXP 0 @13 12
		endif
		PLAYER_CLOSE_DIALOG1 0 0
	endif
endproc

proc 160
	dialog 1
	add_talk $_npc$:
	add_talk #b#b��̫�����ҵ����ѣ������ﻹ��һЩ�µ����ӣ���������Ҳ��ܽ�������͸��㣬�������Ƕ����׵õ��Ķ�����̫��ϧ�����ԣ�����Ҫ����������Ϊ���ۡ�
	add_talk #b#b�ر����ѣ������Ƕ�ͯ���ڼ䣬���껪���ڿ��������ҿ��Ƿ���5��Ӵ��
	add_talk #b#b
	option 161 #��Ը��2000���һ�һ������������Ļ��ᣡ
	
	talk
endproc

;��2000���ң�����1����ɫ����
proc 161 
	strvar ret
	get_id_player_info 0 14 ret
	if @ret >= 2000
		call 201
	else
		call 204
		add_sys_msg_start
		add_sys_msg ���Ҳ��㣡
		set_sys_msg_mode 4 0 0
		msg_info
	endif
endproc

proc 201
	DEC_MONEY_TEST 0 2000 20 10
	if 10 = 0
		GET_PACK_SPACE 0 11
	
		if 11 < 1
			add_sys_msg_start
			add_sys_msg �����ռ䲻�㣡
			set_sys_msg_mode 4 0 0
			msg_info
		else
			DEC_MONEY 0 2000 20 12
			if 12 = 0
				ADD_BIND_ITEM_NUM_INPAK 5241520 1 1 13
				add_sys_msg_start
				add_sys_msg �������
				set_sys_msg_mode 3 0 0
				msg_info
			endif
		endif
	else
		add_sys_msg_start
		add_sys_msg �������Ҳ��㡣
		set_sys_msg_mode 4 0 0
		msg_info
	endif
endproc

proc 162
    dialog 1
	add_talk $_npc$:
	add_talk #b#b��̫�����ҵ����ѣ������ﻹ��һЩ�µ����ӣ���������Ҳ��ܽ�������͸��㣬�������Ƕ����׵õ��Ķ�����̫��ϧ�����ԣ�����Ҫ����������Ϊ���ۡ�
	add_talk #b#b �ر����ѣ������Ƕ�ͯ���ڼ䣬���껪���ڿ��������ҿ��Ƿ���5��Ӵ��
	add_talk #b#b
	option 163 #��Ը��2000���һ�һ����鲼����Ļ��ᣡ
	
	talk
endproc

;��2000���ң�����1����ɫ����
proc 163
	strvar ret
	get_id_player_info 0 14 ret
	if @ret >= 2000
		call 203
	else
		call 204
		add_sys_msg_start
		add_sys_msg ���Ҳ��㣡
		set_sys_msg_mode 4 0 0
		msg_info
	endif
endproc

proc 203
	DEC_MONEY_TEST 0 2000 20 10
	if 10 = 0
		GET_PACK_SPACE 0 11
	
		if 11 < 1
			add_sys_msg_start
			add_sys_msg �����ռ䲻�㣡
			set_sys_msg_mode 4 0 0
			msg_info
		else
			DEC_MONEY 0 2000 20 12
			if 12 = 0
				ADD_BIND_ITEM_NUM_INPAK 5241521 1 1 13
				add_sys_msg_start
				add_sys_msg �������
				set_sys_msg_mode 3 0 0
				msg_info
			endif
		endif
	else
		add_sys_msg_start
		add_sys_msg �������Ҳ��㡣
		set_sys_msg_mode 4 0 0
		msg_info
	endif
endproc


proc 204
	call 1
endproc
