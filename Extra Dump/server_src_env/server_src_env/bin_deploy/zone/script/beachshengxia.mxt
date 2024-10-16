;盛夏商人

proc 1

	DIALOG 1

	add_talk $_npc$:
	add_talk #b#b在房间里呆的久了，就会让全身看起来泛着不健康的惨白，让身材变形，让人失去好奇心，让亲朋的关系变淡，所以，多到阳光下走走，多晒晒太阳，心情就会好，生命也会因此而充实！
	
	
	option 120 #(买卖)打开商店
	
	talk
	
endproc 

proc 120
	OPEN_NPC_SHOP 
endproc 