proc 1
	dialog 1
	add_talk $_npc$:
	add_talk #b#b���͵���ʿΪ�������˵ķ��������˲���ĥ��Ĺ��ף�ֵ�˼ѽڣ�������������׼���˷ḻ����Ʒ���������ͣ�ף��λ��ʿ���彡�����������⣡
	add_talk #b#b#c005ebe5��6�գ����;ͻ��뿪ʱ��ǣ���ƫԶ��������ο�ʹ�����
	add_talk #b#b#c186506�����¸�֮�񣨼��껪�����õ����ҷ�5����
	
	OPTION 300 ο�ʹ������񣨽����ر�����
	OPTION 100 ����ͬ��֮����ȡ���߽�����
	OPTION 200 ��������֮��ս����¶һ�˫��������
	
	talk
endproc

proc 300
	dialog 1
	add_talk $_npc$:
	add_talk #b#b���ڴ�һ������ʱ��������������������Ͷ����Ƿܶ��ڵ�һ�ߣ��������׳�һ�㰮������л�����͵����ǻ��
	
	task_add_option 4440 1 400
	task_add_option 4441 1 401
	task_add_option 4442 1 402
	task_add_option 4443 1 403
	task_add_option 4444 1 404
	task_add_option 4445 1 405
	
	TASKCYCLE_ADD_OPTION 137 1 440
	
	OPTION 1 ����
	
	talk
endproc

proc 100	
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

	
	dialog 1
	add_talk $_npc$:
	add_talk #b#bÿ������6����������ʱ�䣬��������ʱ����ۼơ�
	add_talk #b#b����������ʱ��Ϊ��$@10$ʱ$@11$��
	GET_PLAYER_DAILY_FLAG 0 9 20
	GET_PLAYER_DAILY_FLAG 0 10 21
	GET_PLAYER_DAILY_FLAG 0 11 22
	if 20 = 0
	OPTION 101 ��ȡ����1Сʱ����
	endif
	
	if 21 = 0
	OPTION 102 ��ȡ����2Сʱ����
	endif
	
	if 22 = 0
	OPTION 103 ��ȡ����4Сʱ����
	endif
	
	option 1 ����
	talk
endproc

proc 101
	GET_DAYONLINE 1 10

	if 10 < 3600
		dialog 1
		add_talk $_npc$:
		add_talk #b#b�Բ�������������ʱ�䲻��1Сʱ��������ȡ������
		option 100 ����
		talk
	else
		strvar Ret 
		SET_PLAYER_DAILY_FLAG Ret 9 1
		if Ret = 0
			GET_ID_PLAYER_INFO 0 19 11
			DEV 11 10000
			MUL 11 30
			
			ADD_EXP 0 @11 12
		endif
		PLAYER_CLOSE_DIALOG1 0 0
	endif
endproc

proc 102
	GET_DAYONLINE 1 10

	if 10 < 7200
		dialog 1
		add_talk $_npc$:
		add_talk #b#b�Բ�������������ʱ�䲻��2Сʱ��������ȡ������
		option 100 ����
		talk
	else
		strvar Ret 
		SET_PLAYER_DAILY_FLAG Ret 10 1
		if Ret = 0
			GET_ID_PLAYER_INFO 0 19 11
			DEV 11 10000
			MUL 11 50
			
			ADD_EXP 0 @11 12
		endif
		PLAYER_CLOSE_DIALOG1 0 0
	endif
endproc

proc 103
	GET_DAYONLINE 1 10

	if 10 < 14400
		dialog 1
		add_talk $_npc$:
		add_talk #b#b�Բ�������������ʱ�䲻��4Сʱ��������ȡ������
		option 100 ����
		talk
	else
		strvar Ret 
		SET_PLAYER_DAILY_FLAG Ret 11 1
		if Ret = 0
			GET_ID_PLAYER_INFO 0 19 11
			DEV 11 10000
			MUL 11 100
			
			ADD_EXP 0 @11 12
		endif
		PLAYER_CLOSE_DIALOG1 0 0
	endif
endproc

proc 200
	dialog 1
	add_talk $_npc$:
	; add_talk #b#b���Ǵ�����������Ϊ�ܶ���ս���е���ʿ��׼���ĺ�������ÿ���㶼��������������ս�����Ӷһ�������������ս����»������潱�£������Զһ���1000��������
	add_talk #b#b���Ǵ�����������Ϊ�ܶ���ս���е���ʿ��׼���ĺ�������ÿ���㶼��������������ս����¶һ�������
	add_talk #b
	add_talk #b #c005ebeÿ�οɶһ�1000��������ܹ��ɶһ�18��
	; add_talk #b#b#cff4e00���潱��#nͨ������#cff4e00��ɳ�Ƕ���#n���
	add_talk #b#b#b#bս�����ͨ���������޷籩���
	add_talk #b#b�������˰ͷ��ṩ��������ȡ���µķ���#n
	
	; GET_PLAYER_DAILY_FLAG 0 13 20
	; if 20 = 0
	strvar zhi
	GET_ROLE_VAR 73 8 0 zhi
	add_talk #b
	add_talk #b#b#b#b#b#b �ѳɹ��һ���#c186506$@zhi$��#n
	
	if zhi >= 18
		add_talk #b#b#b #c186506����ȫ���һ�18�λ��18000����#n
	endif
	if zhi < 18
		OPTION 201 1öս����� �һ�1000����
	endif
		; OPTION 202 1ö���潱�� �һ� 1000����
	; else
		; OPTION 210 #�������Ѿ��ɹ��һ���������
	; endif
	option 1 ����
	talk
endproc

proc 201
	strvar zhi
	GET_ROLE_VAR 73 8 0 zhi
	
	if zhi < 18
		GET_ITEM_NUM_INPAK 5060003 0 11
		if 11 > 0
			DEC_ITEM_NUM_INPAK 5060003 1 12
			if 12 = 0
				; strvar Ret 
				; SET_PLAYER_DAILY_FLAG Ret 13 1
				ADD_MONEY 0 1000 76 Ret
				if Ret = 0
					add zhi 1
					SET_ROLE_VAR 73 8 @zhi 0
				endif
			endif
			PLAYER_CLOSE_DIALOG1 0 0
		else
			GET_ID_PLAYER_INFO 0 24 13
			if 13 > 0
				; SET_PLAYER_DAILY_FLAG 0 13 1
				strvar ret2
				DEC_MONEY 0 1 77 ret2
				if ret2 = 0
					ADD_MONEY 0 1000 76 Ret
					if Ret = 0
						add zhi 1
						SET_ROLE_VAR 73 8 @zhi 0
					endif
				endif
				PLAYER_CLOSE_DIALOG1 0 0
			else
				dialog 1
				add_talk $_npc$:
				add_talk #b#b���Ľ����в�δ����ս����£���ʱ�޷��һ�������
				option 200 ����
				talk
			endif
		endif
	endif
endproc

proc 202
	GET_ITEM_NUM_INPAK 5060009 0 11
	if 11 > 0
		DEC_ITEM_NUM_INPAK 5060009 1 12
		if 12 = 0
			strvar Ret 
			SET_PLAYER_DAILY_FLAG Ret 13 1
			ADD_MONEY 0 1000 76 0
		endif
		PLAYER_CLOSE_DIALOG1 0 0
	else
		GET_ID_PLAYER_INFO 0 25 13
		if 13 > 0
			DEC_MONEY 0 1 78 0
			SET_PLAYER_DAILY_FLAG 0 13 1
			ADD_MONEY 0 1000 76 0
			PLAYER_CLOSE_DIALOG1 0 0
		else
			dialog 1
			add_talk $_npc$:
			add_talk #b#b�Բ������ı����ﻹû�����潱�¡�
			option 200 ����
			talk
		endif
	endif
endproc

proc 400
	task_do_option 4440 1
endproc

proc 401
	task_do_option 4441 1
endproc

proc 402
	task_do_option 4442 1
endproc

proc 403
	task_do_option 4443 1
endproc

proc 404
	task_do_option 4444 1
endproc

proc 405
	task_do_option 4445 1
endproc

proc 440
	TASKCYCLE_DO_OPTION 137 1
endproc
