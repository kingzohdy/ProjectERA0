proc 1
	DIALOG 1
	strvar ClanRet
	is_in_my_clan_city  ClanRet
	if ClanRet = 0
		add_talk $_npc$:
		add_talk #b#b��ʵ�������кܶ�ö�����ֻ�����ǲ�֪������Ҳ��Ҫ������˵Ŷ����
	
	else	
	
		add_talk $_npc$:
		add_talk #b#b����ô�๱�׶��ˣ�����ѡ��Ҫ����Ʒ�ɣ������кܶ�ö���Ŷ
		add_talk #b#b
		add_talk #b#b#c186506ֻҪ���Ĺ���ѧϰ�˶�Ӧ�ĿƼ�������ȥ��������Ϳ����������ﹺ������ˣ��������������Ŀɲ���ˮ��������#n
		
		option 150 #���׶��̵�
	endif
	talk
	
endproc


proc 150
	OPEN_NPC_SHOP 
endproc 
	
	
	
	