

proc 1
	DIALOG 1
	strvar ClanRet
	is_in_my_clan_city  ClanRet
	if ClanRet = 0
		add_talk $_npc$:
		add_talk #b#b别看我位置偏僻，我可以给公会制造好多好东西……
	
	else	
		add_talk $_npc$:
		add_talk #b#b你好天行者，我是这里的负责人，管理公会大部份器械的生产制作，例如蒸汽器械、战车等等。
		add_talk #b#b
		strvar Ret
		GET_CLAN_BUILDING_LV Ret 0 2
		if Ret = 100 or Ret = 0
;		add_talk #b#b#c186506当前工厂等级为：0级#n		
		endif
		if Ret = 101 or Ret = 1
;		add_talk #b#b#c186506当前工厂等级为：1级#n		
		endif
		if Ret = 102 or Ret = 2
;		add_talk #b#b#c186506当前工厂等级为：2级#n		
		endif	
		
		if Ret > 0 and Ret != 100
;			option 119 #升级工厂
			option 120 #工厂制造
			
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



