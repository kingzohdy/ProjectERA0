proc 1
	dialog 1
	add_talk $_npc$:
	add_talk #b#b参见酋长大人。我刚刚听到海桑的惨叫声，原来是酋长大人，您怎么……
	add_talk #b#b……多谢酋长大人救命！
	TASK_STAT 1323 0 20
	if 20 = 2 
		option 100 他们为什么要掳走你？
	endif
	talk
endproc
proc 100
	dialog 1
	add_talk $_npc$:
	add_talk #b#b他们想打探连命术仪式的秘密，以及纳巴的下落。不过请酋长大人放心，我并没有告诉他们纳巴被您带到战鼓村秘密据点的事情。

	option 102 你先回战鼓村传我命令，严加防范！
	
	talk
endproc
proc 101
	dialog 1
	add_talk $_npc$:
	add_talk #b#b他们想打探连命术仪式的秘密，以及纳巴的下落。不过请酋长大人放心，我并没有告诉他们纳巴被您带到战鼓村秘密据点的事情。

	option 102 很好，你先回战鼓村，传我命令，严加看管纳巴，千万别让他逃了！
	
	talk
endproc
proc 102
	dialog 1
	add_talk $_npc$:
	add_talk #b#b属下遵命！
	
	 option 103 #嗯！
	task_fini 1323 0
	talk

endproc
proc 103
	return
endproc
;proc 2
;	get_trig_task 1 2
;	if 2 = 1308
;		task_fini 1308 1
;	endif
;
;endproc

