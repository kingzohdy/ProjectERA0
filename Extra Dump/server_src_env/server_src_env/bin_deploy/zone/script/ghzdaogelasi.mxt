

proc 1
	DIALOG 1

	add_talk $_npc$:
	add_talk #b#b��õķ������ǽ������Ƿ�����Ĺؼ�֮���������ӳ��ڵĽ������������ҽ����˵���ʮ��ľ��飬�㶨����ʵ����̫�����ˡ����ҵ�û��ġ�
	
	strvar Ret
	IS_WORLD_CITY_OWNER Ret
	if Ret = 1
		option 200 #�Ƿ�����
		option 202 #��ʱ���
	endif
	option 201 #�ر�
	
	talk
endproc 
proc 200
	open_ui 59
endproc
proc 201
	return
endproc
proc 202
	TRANSMIT 1000 26600	28100 0

endproc
