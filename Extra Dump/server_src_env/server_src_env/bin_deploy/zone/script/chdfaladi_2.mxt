
;小方
proc 1

	dialog 1
	add_talk $_npc$:
	add_talk #b#b嗨，$_player$，你想现在参加科技冲冲冲活动？
	add_talk #b#b真抱歉，我需要用一周的时间来整理之前收集到的物资，将它们分类打包。也许你不知道，有些东西是不可以放在一起的，比如烂苹果和熟透了的柿子……
	add_talk #b#b所以，能麻烦你等我整理完了再来吗？
	
		option 150 #再见
	talk
endproc
proc 150
	return
endproc









