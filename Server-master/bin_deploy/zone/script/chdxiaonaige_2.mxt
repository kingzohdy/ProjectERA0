
;小奶哥
proc 1

	dialog 1
	add_talk $_npc$:
	add_talk #b#b你也是来忆当年的吗？可惜……
	add_talk #b#b可惜，你来晚了，活动已经结束了！
	add_talk #b#b当然，也可以说你来早了，下一次活动还没开始！
	add_talk #b#b你认为早到比迟到好？NO，NO，NO，早到与迟到同样。
	add_talk #b#b很多人都觉得迟到的人没有时间观念，是对别人的不负责任，殊不知早到的人是对自己不负责！

	
		option 150 #再见
	talk

endproc

proc 150
	return
endproc
