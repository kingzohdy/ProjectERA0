;新手村装备商人

proc 1

	dialog 1
	add_talk $_npc$:
	add_talk #b#b我喜欢早上起来时一切都是未知的，不知道会遇见什么人，会有什么样的结局。也许会有一个丢三落四的小迷糊再出现在我这里，怯生生的问，有没有一把与她丢的相同或者类似的法杖……
	

	
	OPTION 110 #(买卖)打开商店 	
	talk
	
endproc

proc 110
    OPEN_NPC_SHOP
endproc

