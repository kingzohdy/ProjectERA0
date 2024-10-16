;牧牛决赛,子里2
proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b这里是牛仔很忙决赛的终点，不是成功之路的终点，更不是人生之路的终点。
	add_talk #b#b从起点到终点，有时候会有多条路供你选择，就像这初赛，刚走左边突然觉得走右边可能会更近；刚走向右边又觉得走左边可能会快一些；从左边走到终点成绩不佳，就想若是之前走了右边，也许会很顺序；从右边到达终点又会想，如果我从左边走，也许成绩会更好……
	
	task_add_option 30023 1 100
	talk
endproc
proc 100
	task_do_option 30023 1
endproc
