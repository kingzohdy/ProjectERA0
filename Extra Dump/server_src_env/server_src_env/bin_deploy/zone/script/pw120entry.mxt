;80����,���npc,������
proc 1
	strvar l
	GET_PLAYER_BASE_ATT 1 0 l 0
	if l < 80
		dialog 1
		ADD_TALK $_npc$:
		ADD_TALK #b#b��ã�#Name#,�ܵ�������������Ѿ������˲��ٵ��������⽫����һ�������ѵ���ս��
		talk
		
	else
		dialog 1
		ADD_TALK $_npc$:
		ADD_TALK #b#b��ã�#Name#,�ܵ�������������Ѿ������˲��ٵ��������⽫����һ�������ѵ���ս��
		ADD_TALK #b#bԶ��Ҫ���������Ѿ�������פ������ã����ﲡ�����ӣ����������ټ�����Ща���������ֺ��������Ѿ��಻�����ˡ�
		option 100 #���븱��
		talk
		
	endif
	
endproc
proc 100
;	enter_pworld 120 0 1
	
	OPEN_PWORLD_UI 5
endproc
