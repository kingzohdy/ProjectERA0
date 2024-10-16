;302200	乔治


proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b尊敬的#Name#您好。
	add_talk #b#b在这气氛凝重的战场中，我这一定有您需要的东西。
	
		OPTION 110 #(买卖)打开商店
	
	talk
endproc 

proc 110
    OPEN_NPC_SHOP
endproc
