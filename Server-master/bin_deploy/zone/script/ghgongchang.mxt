

proc 1
	DIALOG 1
	strvar ClanRet
	is_in_my_clan_city  ClanRet
	if ClanRet = 0
		add_talk $_npc$:
		add_talk #b#b����λ��ƫƧ���ҿ��Ը���������ö�ö�������
	
	else	
		add_talk $_npc$:
		add_talk #b#b��������ߣ���������ĸ����ˣ�������󲿷���е����������������������е��ս���ȵȡ�
		add_talk #b#b
		strvar Ret
		GET_CLAN_BUILDING_LV Ret 0 2
		if Ret = 100 or Ret = 0
;		add_talk #b#b#c186506��ǰ�����ȼ�Ϊ��0��#n		
		endif
		if Ret = 101 or Ret = 1
;		add_talk #b#b#c186506��ǰ�����ȼ�Ϊ��1��#n		
		endif
		if Ret = 102 or Ret = 2
;		add_talk #b#b#c186506��ǰ�����ȼ�Ϊ��2��#n		
		endif	
		
		if Ret > 0 and Ret != 100
;			option 119 #��������
			option 120 #��������
			
		endif
	endif
	talk
	
endproc 
proc 119
	open_ui 23
endproc

proc 120
	open_ui 49
endproc



