; 直升机


proc 1
	dialog 1
	add_talk $_npc$:
	
	add_talk #b#b由于能源有限，只能近距离飞行。
	
		
		option 160 #乘坐直升机
	talk
endproc
proc 160 
	AUTOFLY 125 1
endproc 
