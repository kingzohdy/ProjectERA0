

proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#bʲô������Ʒ�������ˣ�
	add_talk #b#b��ǿ�󣬸�ϡ�٣������ˣ�
	add_talk #b#bר���̵ֻ꣬������ϡ�е���Ʒ���߹�·����Ҫ����ˣ�
	add_talk #b#bû��������ƷֻҪ���ٵ�Ǯ���Ϳ�����������Ψһ�ģ��������ʱװ�������û�п����Ǻ��ٵ�Ǯ����ȡ�������Ψһ��ʱװ�����
	add_talk #b#b������Ҫ������һ�䣬���ǿ�����Ʒ��������ͨ���ܹ���㴩���ģ�ֻ�������ݴﵽ��һ����Ҫ�󣬲���ʹ�����ǿ�����Ʒ��

	strvar Ret Num
	GET_WORLD_CITY_KEEP_NUM Ret Num	
	strvar RetClan
	HAS_MASTER_WORLD_CITY RetClan
	
	if Ret = 0 and RetClan = 1
		add Num 1
		add_talk #b#c186506$_WorldCityClanName$�����Ѿ�����ռ��ʱ���$@Num$�Ρ�#n
	endif
	add_talk #b#b
	
	;strvar Ret
	;IS_WORLD_CITY_OWNER Ret
	;if Ret = 1
	if RetClan = 1
		option 200 #���̵�
	endif
	option 201 #�ر�
	
	talk
endproc 
proc 200
	OPEN_NPC_SHOP 
endproc
proc 201
	return
endproc
proc 202
	TRANSMIT 1000 26600	28100 0

endproc
