proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b我手上有一些古老的附魔配方，一般人是弄不到的。什么？你问我从哪里弄来的？这可是商业秘密…

		option 110 #附魔商店
	talk
	
endproc

proc 110
	OPEN_NPC_SHOP
endproc  