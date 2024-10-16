
;米露
proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b每次让艾尔玛出去都不认真帮忙，真是气死我这个做姐姐的了。
	add_talk #b#b客人随便看看啊，我这里有任何你想要的材料和工具，保证全城最低价出售！
	add_talk #b#b#c186506出售与“科技生产”相关的工具与材料#n
	
		OPTION 110 #打开商店


	talk
	
endproc 

proc 110

	OPEN_NPC_SHOP 
endproc 