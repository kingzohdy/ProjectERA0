

proc 1
	DIALOG 1
	
	strvar ClanRet
	is_in_my_clan_city  ClanRet
	if ClanRet = 0
		add_talk $_npc$:
		add_talk #b#bС�ӣ���͵�����𣿿�����Զ�㣬�����Ҿͽ��ˣ�
	
	else
	
		add_talk $_npc$:
		add_talk #b#b��ã��ҽ������¼�������ʵ�����֧��������Ӧ��û�˱�����Ҫ���������ˡ�
		add_talk #b#b
		add_talk #b#b#c186506�����Աߵ��Ǹ���������������ĵ������Ŀɱ�����#n#cff00ff����2~5��#n#c186506�أ���������Ҫ�����Լ���������ſ��Ի�ȡ�ĵ���
		add_talk #b#b
		add_talk #b#b#c8a8a8aС��ʾ����������ĵ���Ĭ���������ģ���Ҫ�᳤������ſɻ�ȡ#n
		add_talk #b#b#c8a8a8a����״̬��ֻ�л᳤�ɻ�ȡ#n
		add_talk #b#b#c8a8a8a����״̬����Ա���Ϲٽ׿ɻ�ȡ#n
		add_talk #b#b#c8a8a8a����״̬�����й����Ա���ɻ�ȡ#n
	
		option 120 #������Դ����
		option 140 #��ȡ��¼
;		option 121 #�ύ��
;		option 122 #�ύ��Դ
	endif	
	talk
endproc 

proc 120

	open_ui 47
endproc 
proc 121
	GIVEBACK_CLAN_MACHINE 0 0
endproc
proc 122
	LAYDOWN_CLAN_ORE 0 0
endproc 

proc 140
	open_ui 72
endproc