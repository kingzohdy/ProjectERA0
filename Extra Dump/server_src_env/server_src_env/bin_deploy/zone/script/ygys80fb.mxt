
proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b这是一座封存了无数年的远古要塞,里面埋葬着千万年的文明积淀......
	
	
	option 300 #进入远古要塞
	
	talk
	
endproc


proc 300


	OPEN_PWORLD_UI 5



endproc
