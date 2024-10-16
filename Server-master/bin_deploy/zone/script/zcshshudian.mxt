proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b我拥有成千上万册的藏书，但我的良知告诉我不应该独占这些知识，所以我把一些古时的战斗技巧抄录了下来，提供给英雄们使用

		option 110 #书店
	talk
	
endproc

proc 110
	OPEN_NPC_SHOP
endproc  