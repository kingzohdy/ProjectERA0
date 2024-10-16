	
proc 1

	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b兰姆好帅哦，好像布拉德.皮特！换完装备去帮我传个话给他吧…
	
	OPTION 110 #(买卖)打开商店 


	talk


endproc

proc 110
	 OPEN_NPC_SHOP
endproc



