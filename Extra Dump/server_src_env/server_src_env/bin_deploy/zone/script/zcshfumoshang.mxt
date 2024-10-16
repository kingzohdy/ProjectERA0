proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b我年轻的时候也是一个出色的附魔师，直到我受了一次重伤。现在我以贩卖多年收集到的配方为生。

		option 110 #附魔商店
	talk
	
endproc

proc 110
	OPEN_NPC_SHOP
endproc  