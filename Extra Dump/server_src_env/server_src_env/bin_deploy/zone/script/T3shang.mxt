	
proc 1

	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b女人说我帅，男人说我只是帅，但总是没有人在意我的内在，唉…你要换什么？
	
	OPTION 110 #(买卖)打开商店 


	talk

endproc

proc 110
	 OPEN_NPC_SHOP
endproc


