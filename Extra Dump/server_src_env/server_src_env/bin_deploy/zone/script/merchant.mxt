;布列特脚本
;11--打开商店事件

proc 1
	dialog 1
	
	add_talk 小白说了：天哪，你真高！
	option 11 #打开商店
	talk
endproc

proc 11
	open_npc_shop
endproc
