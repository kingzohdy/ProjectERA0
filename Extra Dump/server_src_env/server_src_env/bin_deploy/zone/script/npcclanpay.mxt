

proc 1
	
	strvar Ret
	IS_WORLD_CITY_OWNER Ret
	if Ret = 1
		call 100
	else
		call 200
	endif
endproc 

proc 100
	DIALOG 1

	GET_PLAYER_BASE_ATT 1 0 2 0
	get_world_city_keep_num 1 3
	clan_pay 0 1 4

	add 3 1

	add_talk $_npc$:
	add_talk #b#b�𾴵�ʱ��ǰ�����Ա����Ը��Ϊ��Ч�͡�
	add_talk #b#b
	add_talk #b#b�����ڹ����Ѿ�����ռ��ʱ���$@3$�Σ�
;	add_talk #b#b��������$@2$�ȼ���
;	add_talk #b#b����ȡռ���ʽ�$@4$���ҡ�

	; option 101 #��ȡռ���ʽ�	
	option 99 #�ر�
	
	talk
endproc

proc 101
	clan_pay 0 1 0
endproc

proc 200
	DIALOG 1

	add_talk $_npc$:
	add_talk #b#b��Ϊһ�����еĸ����ˣ��ҿ���ӵ�кܶ���Ȩ���������������ʲô���İ�����ȫ���ҵ����顣
	add_talk #b#bС�ӣ�����Զһ�㣬����Ϊ˭������ָʹ�Ҹ��Ÿ��ǵġ��ң��ָ���ˣ�ֻ����Ϊ��ʱ��ǰ������ṩ���񡣲�������ʲôʱ���Ϊ������һԱ���һ���Ը��Ϊ��Ч�͵ġ�
	
	option 99 #�ر�
	
	talk
endproc


