;�±�ǩ����

proc 1
	dialog 1
	add_talk $_npc$:
	add_talk #b#b��λ�������˵ľ��������Ԫ640��������ĩ���۹������Ƿ��������ν�����������������������������������������ڱ�Դʮ�ֶ�ȱ��Ϊ�˱������ǵļ�԰��Ϊ������������ӵ���Ҹ���ƽ�����磬�������ǰɣ�

	TASK_STAT 1051 1 3
	if 3 = 2
		option 100 #ǩ��
	endif
	talk
endproc

proc 100
	strvar X Y
	GET_PLAYER_MAPINFO 0 0 0 X Y
	
	effect 2 0 playUiEffect(0,0,2,"effect\\entity\\9576_16.ent")	
	
	effect 2 0 playEffct($@X$,$@Y$,"effect\\entity\\9576_18.ent")
	TASK_FINI 1051 0
	
endproc 


