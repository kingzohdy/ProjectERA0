; ֱ����


proc 1
	dialog 1
	add_talk $_npc$:
	strvar Gend
	GET_PLAYER_BASE_ATT 0 Gend 0 0
	if Gend = 1
		
		add_talk #b#b˧�磬����ȥ�۲�̨�����Ӵ��
	else
		
		add_talk #b#b��Ů������ȥ�۲�̨�����Ӵ��
	endif
	
		strvar Ret
		task_stat 2114 1 Ret
		if Ret = 3
			option 150 #����ֱ����
		endif
		
	talk
endproc
proc 150
	AUTOFLY 124 10
endproc 
