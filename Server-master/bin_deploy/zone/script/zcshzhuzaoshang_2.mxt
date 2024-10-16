proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b没有什么事情比挥舞手上锤子的感觉来得实在，看着材料逐步打造成装备的感觉真的很棒！你要来试试吗？
	
		option 110 #铸造商店
	talk
	
endproc

proc 110
	OPEN_NPC_SHOP
endproc  