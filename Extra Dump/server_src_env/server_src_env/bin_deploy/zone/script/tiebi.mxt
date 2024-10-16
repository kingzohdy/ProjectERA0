;NPC脚本 - 铁臂

;NPC脚本默认执行事件
proc 1
	dialog 1
	add_talk $_npc$:
	add_talk #b#b你们终于赶来了，情况暂时不容乐观，现在冶炼岛上已遍布末世追随者的入侵者，我们必须抓紧时间将其驱逐出岛！
	add_talk #b#b
	add_talk #b#b#c186506你可以通过打开快捷栏上方的“公会活动助手”按钮兑换个人积分奖励，包括查询积分规则与活动剩余时间。#n
	task_add_option 3680 1 100
	talk
endproc

proc 100
	task_do_option 3680 1
endproc



