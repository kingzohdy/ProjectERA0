

proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b一个人就想消灭地人保卫时间城？很多事情不是靠热血就可以的，年轻人。试着建立或加入公会吧，团队和友谊比任何强大的装备更有意义。
	
		option 120 #回到公会城市
		option 121 #提交资源(矿车)
		option 122 #提交资源(背包)

	talk
endproc 
proc 120
;	enter_clan_city	1
	GOTO_CLAN_CITY 0 13000 15000
endproc 
proc 121

	LAYDOWN_CLAN_ORE 0 0
endproc 
proc 122
	COMMIT_CLAN_ITEM 1 0 0

endproc
