
proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b�������������ʲô����������±���������ܡ���ϲ������ô�����Լ�������ȥ��һ����õ��Ͽɣ�����Ҳ�������ġ�

	add_talk #b#b����Ϊ�˷����ң�������Ҳ���ṩ��������ġ��Һ�æ�����ܵ�ʹ�á�
		task_add_option 3304 1 100
		task_add_option 3305 1 101

		strvar lv
		GET_PLAYER_BASE_ATT 0 0 lv 1
		
		if lv >= 50
			option 200 ��Ҫ������ɡ����뻻������
		endif
		
		if lv >= 35 and lv < 45
			option 210 ��Ҫ������ɡ����顿��Ŭ��˹���
		endif
	
		if lv >= 45 and lv < 55
			option 220 ��Ҫ������ɡ����顿ʮ���ſ�
		endif
	
		if lv >= 55 and lv < 65
			option 230 ��Ҫ������ɡ����顿Խ������
		endif
		
		if lv >= 65 and lv < 75
			option 240 ��Ҫ������ɡ����顿����ʥ��
		endif
		
		if lv >= 75 and lv < 85
			option 250 ��Ҫ������ɡ����顿�γ�
		endif
	talk
endproc 
proc 100
	task_do_option 3304 1
endproc 
proc 101
	task_do_option 3305 1
endproc 
proc 200
	strvar lv
	GET_PLAYER_BASE_ATT 0 0 lv 1
	if lv >= 50 and lv < 70
		PLAYER_BUSY_UI 14 1
	endif
	if lv >= 70
		PLAYER_BUSY_UI 15 1
	endif
endproc
proc 210
	strvar lv
	GET_PLAYER_BASE_ATT 0 0 lv 1
	if lv >= 35 and lv < 45
		PLAYER_BUSY_UI 16 1
	endif
endproc 
proc 220
	strvar lv
	GET_PLAYER_BASE_ATT 0 0 lv 1
	if lv >= 45 and lv < 55
		PLAYER_BUSY_UI 17 1
	endif
endproc 
proc 230
	strvar lv
	GET_PLAYER_BASE_ATT 0 0 lv 1
	if lv >= 55 and lv < 65
		PLAYER_BUSY_UI 18 1
	endif
endproc 
proc 240
	strvar lv
	GET_PLAYER_BASE_ATT 0 0 lv 1
	if lv >= 65 and lv < 75
		PLAYER_BUSY_UI 19 1
	endif
endproc 
proc 250
	strvar lv
	GET_PLAYER_BASE_ATT 0 0 lv 1
	if lv >= 75 and lv < 85
		PLAYER_BUSY_UI 20 1
	endif
endproc 