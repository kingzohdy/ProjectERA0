proc 1
	dialog 1
	add_talk $_npc$:
	add_talk #b#b我是谁，从何来向何去，这些都不重要，请小心那些在四处巡逻的失落灵。如果你想活着离开，或许需要我手上的东西
	add_talk #b#b我在此早已恭候多时，失落之域欢迎你的到来。
	
	OPTION 200 #打开商店
	
	
	talk
endproc 

proc 200
	OPEN_NPC_SHOP 
endproc

