

proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b还在为找不到生活材料而痛苦吗，我这里出售低级的生活材料，价格优惠哦，走过路过千万不要错过！！


	OPTION 110 #(买卖)打开商店 

	talk

endproc 

proc 110
    	OPEN_NPC_SHOP
endproc


