
proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b���ã��������̻����·���Ϊ���ṩ������æ��ûʱ����������ѣ����ڿ��Թ�Ӷ�����������������ɡ�
	add_talk #b#b��ɺ���Ի�û��Ӧ�Ľ�����
	add_talk #b#b
	add_talk #b#b��ʱֻ�ܰ�����ɱ��뻻�������������񡢻�������Ʒ�����������
	
	strvar lv
	GET_PLAYER_BASE_ATT 0 0 lv 1
	
	if lv >= 50
		option 100 ��Ҫ������ɡ����뻻������
	endif
		
	if lv >= 35 and lv < 45
		option 110 ��Ҫ������ɡ����顿��Ŭ��˹���
	endif
	
	if lv >= 45 and lv < 55
		option 120 ��Ҫ������ɡ����顿ʮ���ſ�
	endif
	
	if lv >= 55 and lv < 65
		option 130 ��Ҫ������ɡ����顿Խ������
	endif
	
	if lv >= 65 and lv < 75
		option 140 ��Ҫ������ɡ����顿����ʥ��
	endif
	
	if lv >= 75 and lv < 85
		option 150 ��Ҫ������ɡ����顿�γ�
	endif
	
	if lv > 40
		option 160 ��Ҫ�������һ�Ρ����Ʒ���
	endif
	
	strvar Ret1 Pos
	player_clan_pos_info Ret1 0 0 0 Pos
	
	if Ret1 = 0 and Pos != 0
		if lv > 40
			option 170 ��Ҫ������ɡ���������
		endif
	endif
	
	talk
endproc 

proc 100
	strvar lv
	GET_PLAYER_BASE_ATT 0 0 lv 1
	if lv >= 50 and lv < 70
		PLAYER_BUSY_UI 14 1
	endif
	if lv >= 70 and lv <= 120
		PLAYER_BUSY_UI 15 1
	endif
endproc
proc 110
	strvar lv
	GET_PLAYER_BASE_ATT 0 0 lv 1
	if lv >= 35 and lv < 45
		PLAYER_BUSY_UI 16 1
	endif
endproc 
proc 120
	strvar lv
	GET_PLAYER_BASE_ATT 0 0 lv 1
	if lv >= 45 and lv < 55
		PLAYER_BUSY_UI 17 1
	endif
endproc 
proc 130
	strvar lv
	GET_PLAYER_BASE_ATT 0 0 lv 1
	if lv >= 55 and lv < 65
		PLAYER_BUSY_UI 18 1
	endif
endproc 
proc 140
	strvar lv
	GET_PLAYER_BASE_ATT 0 0 lv 1
	if lv >= 65 and lv < 75
		PLAYER_BUSY_UI 19 1
	endif
endproc 
proc 150
	strvar lv
	GET_PLAYER_BASE_ATT 0 0 lv 1
	if lv >= 75 and lv < 85
		PLAYER_BUSY_UI 20 1
	endif
endproc 
proc 160
	strvar lv
	GET_PLAYER_BASE_ATT 0 0 lv 1
	
	if lv >= 40 and lv < 45
		PLAYER_BUSY_UI 6 1
	endif
	if lv >= 45 and lv < 50
		PLAYER_BUSY_UI 7 1
	endif
	if lv >= 50 and lv < 55
		PLAYER_BUSY_UI 8 1
	endif
	if lv >= 55 and lv < 60
		PLAYER_BUSY_UI 9 1
	endif
	if lv >= 60 and lv < 65
		PLAYER_BUSY_UI 10 1
	endif
	if lv >= 65 and lv < 70
		PLAYER_BUSY_UI 11 1
	endif
	if lv >= 70 and lv < 75
		PLAYER_BUSY_UI 12 1
	endif
	if lv >= 75 and lv <= 120
		PLAYER_BUSY_UI 13 1
	endif
endproc
proc 170
	strvar lv
	GET_PLAYER_BASE_ATT 0 0 lv 1

	if lv > 40 and lv <= 50
		PLAYER_BUSY_UI 24 1
	endif
	if lv > 50 and lv <= 60
		PLAYER_BUSY_UI 25 1
	endif
	if lv > 60 and lv <= 70
		PLAYER_BUSY_UI 26 1
	endif
	if lv > 70 and lv <= 80
		PLAYER_BUSY_UI 27 1
	endif
endproc
   

