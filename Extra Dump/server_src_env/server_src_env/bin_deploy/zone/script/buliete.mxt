;布列特脚本
;11--打开商店事件

proc 1
	dialog 1
	
	add_talk 这几天新手村的动乱已经让我赚了不少,估计销售额快要赶上外面世界了。
	option 11 #(买卖)打开商店
	talk
endproc

proc 11
	open_npc_shop
endproc
