

proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b我真没有可卖的东西了。。。。。。。。


	OPTION 110 #(买卖)打开商店 

	talk

endproc 

proc 110
    	OPEN_NPC_SHOP
endproc


