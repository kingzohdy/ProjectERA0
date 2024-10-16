proc 1
	DIALOG 1
	strvar ClanRet
	is_in_my_clan_city  ClanRet
	if ClanRet = 0
		add_talk $_npc$:
		add_talk #b#b其实我这里有很多好东西，只是他们不知道，你也不要到处乱说哦……
	
	else	
	
		add_talk $_npc$:
		add_talk #b#b有这么多贡献度了？来挑选需要的商品吧，我这有很多好东西哦
		add_talk #b#b
		add_talk #b#b#c186506只要您的公会学习了对应的科技，不用去工厂制造就可以在我这里购买道具了，不过我这里卖的可不是水货啊……#n
		
		option 150 #贡献度商店
	endif
	talk
	
endproc


proc 150
	OPEN_NPC_SHOP 
endproc 
	
	
	
	