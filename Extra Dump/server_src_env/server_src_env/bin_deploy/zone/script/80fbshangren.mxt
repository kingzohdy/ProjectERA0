;80副本商店
proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b我是一个流浪的智能机器人，我是一个生命，一直在寻找我的同类，在这里我发现了已经产生灵智的她，但是却被邪恶的力量所侵袭，希望有勇士能够战胜邪恶的力量，让她恢复清醒！
	add_talk #b#b我能能力不是很强大，但也希望能尽我的绵薄之力来帮助你。
	add_talk #b#b如果你在通关斯巴达尔死亡后，可从我这里传送到斯巴达尔的领地。
	;add_talk #b#b这里有我的一些收藏，希望能够帮到您！
	;option 300 #购买机械
	
		option 200 #传送到斯巴达尔领地
	
	talk
endproc

;proc 300
	;open_npc_shop
;endproc

proc 200
	choose 2 201 202 是否要传送到斯巴达尔领地？
endproc

proc 201
	player_jump 0 8600 18700
endproc
