

proc 1
	DIALOG 1
	strvar ClanRet
	is_in_my_clan_city  ClanRet
	if ClanRet = 0
		add_talk $_npc$:
		add_talk #b#b�Ƽ����ķ�չ�ɹ�ϵ����ҵĺô�Ŷ����
	
	else	
		strvar Ret
		GET_CLAN_BUILDING_LV Ret 0 0	

		add_talk $_npc$:
		add_talk #b#b��ã����Ǹ����������Ľ��ڣ������ǽ����۸���ʵ�ֵĵط����������������һ�пƼ����Ǵ����ﵮ������ϲ����о���
		add_talk #b

		strvar Retcaiji Retgongchang Ret1
		GET_CLAN_USED_TECH_POINT Retcaiji 0 -1
		GET_CLAN_USED_TECH_POINT Retgongchang 0 -2
		GET_CLAN_USED_TECH_POINT Ret1 0 0
		
		
		if Ret = 100 or Ret = 0
			add_talk #b#b#c186506��ǰ�Ƽ����ĵȼ�Ϊ��0�� (������Ҫ2����)#n		
		endif
		if Ret = 101 or Ret = 1
			add_talk #b#b#c186506��ǰ�Ƽ����ĵȼ�Ϊ��1��#n	
		endif
		if Ret = 102 or Ret = 2
			add_talk #b#b#c186506��ǰ�Ƽ����ĵȼ�Ϊ��2��#n	
		endif
;	add_talk #b#b#c186506��ǰͶ��ɼ��Ƽ��㣺$@Retcaiji$#n
;	add_talk #b#b#c186506��ǰͶ������Ƽ��㣺$@Retgongchang$#n
;	add_talk #b#b#c186506��ǰͶ���ܿƼ��㣺$@Ret1$#n

		if Ret > 0 and Ret != 100
			option 120 #����Ƽ�
		endif
		if Ret < 2 and Ret > 0
			option 121 #�����Ƽ�����
		endif
	endif
		
	talk
endproc 
proc 120
	open_ui 45
	
endproc 
proc 121
	open_ui 21
endproc
