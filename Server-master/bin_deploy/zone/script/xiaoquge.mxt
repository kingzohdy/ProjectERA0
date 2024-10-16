proc 1

	dialog 1
	add_talk $_npc$:
	add_talk #b#b为了生存，为了养活我的爱人，我辛辛苦苦的在这里卖点吃的，做做小本生意。
	add_talk #b#b各位英雄，走过路过千万不要错过，要去战鼓村击杀恶贼，一定要带足粮食药品，虽然我这里的东西算不上极品，但至少物廉价美。
	add_talk #b#b对了，如果你要回时间城了，请帮我探望我的爱人，不知道他过的好不好。


	OPTION 101 #(买卖)商店
	
	talk
endproc	

PROC 101
	OPEN_NPC_SHOP
endproc
