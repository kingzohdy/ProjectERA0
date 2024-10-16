

proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b什么样的商品最吸引人！
	add_talk #b#b更强大，更稀少，更便宜！
	add_talk #b#b专属商店，只出售最稀有的商品，走过路过不要错过了！
	add_talk #b#b没错现在商品只要很少的钱，就可以买到世界上唯一的！最特殊的时装与坐骑！您没有看错是很少的钱！换取到特殊的唯一的时装与坐骑！
	add_talk #b#b不过我要提醒你一句，如此强大的物品，不是普通人能够随便穿戴的，只有你的身份达到了一定的要求，才能使用如此强大的物品！

	strvar Ret Num
	GET_WORLD_CITY_KEEP_NUM Ret Num	
	strvar RetClan
	HAS_MASTER_WORLD_CITY RetClan
	
	if Ret = 0 and RetClan = 1
		add Num 1
		add_talk #b#c186506$_WorldCityClanName$公会已经持续占领时间城$@Num$次。#n
	endif
	add_talk #b#b
	
	;strvar Ret
	;IS_WORLD_CITY_OWNER Ret
	;if Ret = 1
	if RetClan = 1
		option 200 #打开商店
	endif
	option 201 #关闭
	
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
