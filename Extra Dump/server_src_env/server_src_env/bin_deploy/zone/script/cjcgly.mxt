

proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b每一份资源都是宝贵的,请大家不要浪费呀!
	add_talk #b#b话说经常在野外看到有人在地上乱丢矿和油,不可饶恕啊!! 什么?! 你是被人干掉后爆掉的?? 那你小子太可怜了...

	

	
	OPTION 110 #(买卖)打开商店

	
	talk
endproc 


PROC 110
	OPEN_NPC_SHOP
ENDPROC


