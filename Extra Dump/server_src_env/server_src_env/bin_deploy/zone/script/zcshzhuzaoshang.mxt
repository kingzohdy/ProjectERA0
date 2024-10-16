proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b即使情形很绝望，也要握紧你的梦想，时刻与战士的荣耀同在。嘛，我虽然没成为战士，也不妨抱着那颗心。

		option 110 #铸造商店
	talk
	
endproc

proc 110
	OPEN_NPC_SHOP
endproc  