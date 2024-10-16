;瓦力

proc 1
	dialog 1
	add_talk $_npc$:
	add_talk #b#b欢迎光临。
	add_talk #b#b请问有什么能帮到您？
	add_talk #b#b…
	add_talk #b#b……
	add_talk #b#b………
	add_talk #b#b谢谢光顾。
	add_talk #b#b期待下一次为您服务。
	add_talk #b#b
	add_talk #b#b #c186506（请自觉投币购买，切勿打砸贩卖机）#n
	
	option 100 #(买卖)打开商店
	
	talk

endproc 

proc 100
	OPEN_NPC_SHOP
endproc
