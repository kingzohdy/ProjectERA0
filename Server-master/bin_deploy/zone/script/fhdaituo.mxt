;70副本传送

proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b一个人就想消灭地人保卫时间城？很多事情不是靠热血就可以的，年轻人。试着建立或加入公会吧，团队和友谊比任何强大的装备更有意义。

		option 120 #进入副本
		option 121 #重置副本

	talk
endproc 
proc 120

;	enter_pworld 105 0 1
;		OPEN_PWORLD_UI 105
		OPEN_PWORLD_UI 3
endproc 
proc 121

	RESET_PWORLD 105
		
endproc
