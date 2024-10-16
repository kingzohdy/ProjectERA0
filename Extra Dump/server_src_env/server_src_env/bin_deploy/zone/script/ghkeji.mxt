

proc 1
	DIALOG 1
	strvar ClanRet
	is_in_my_clan_city  ClanRet
	if ClanRet = 0
		add_talk $_npc$:
		add_talk #b#b科技中心发展可关系到大家的好处哦……
	
	else	
		strvar Ret
		GET_CLAN_BUILDING_LV Ret 0 0	

		add_talk $_npc$:
		add_talk #b#b你好，我是负责管理这里的教授，这里是将理论付诸实现的地方，你所见公会里的一切科技都是从这里诞生，我喜欢这感觉。
		add_talk #b

		strvar Retcaiji Retgongchang Ret1
		GET_CLAN_USED_TECH_POINT Retcaiji 0 -1
		GET_CLAN_USED_TECH_POINT Retgongchang 0 -2
		GET_CLAN_USED_TECH_POINT Ret1 0 0
		
		
		if Ret = 100 or Ret = 0
			add_talk #b#b#c186506当前科技中心等级为：0级 (建设需要2分钟)#n		
		endif
		if Ret = 101 or Ret = 1
			add_talk #b#b#c186506当前科技中心等级为：1级#n	
		endif
		if Ret = 102 or Ret = 2
			add_talk #b#b#c186506当前科技中心等级为：2级#n	
		endif
;	add_talk #b#b#c186506当前投入采集科技点：$@Retcaiji$#n
;	add_talk #b#b#c186506当前投入制造科技点：$@Retgongchang$#n
;	add_talk #b#b#c186506当前投入总科技点：$@Ret1$#n

		if Ret > 0 and Ret != 100
			option 120 #公会科技
		endif
		if Ret < 2 and Ret > 0
			option 121 #升级科技中心
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
