proc 1
	dialog 1
	add_talk $_npc$:
	add_talk #b#b朋友，感觉怎么样？你觉得没意思了么？好吧，你可以通过我离开活动，不过你可是错过了轻松拿经验奖励的机会哦。
	option 300 #返回时间城
	talk
endproc

proc 300
	choose 0 301 302 确定返回时间城吗?
endproc

proc 301
	transmit 1000 39900 36200 1
endproc

proc 302

endproc