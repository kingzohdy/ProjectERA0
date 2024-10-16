proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b谁是豆芽菜？矮冬瓜？长得像豆粒？！我可是鼎鼎大名的炼金商人爱德华！

		option 110 #炼金商店
	talk
	
endproc

proc 110
	OPEN_NPC_SHOP
endproc  